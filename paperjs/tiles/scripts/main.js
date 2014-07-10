/*jslint vars: true, nomen: true, plusplus: true, continue:true, forin:true */
/*global paper, ColorTheme, Point, view, Shape, Path, atob, btoa, ArrayBuffer,
    Uint8Array, Blob, Size, PixelData, Tool, project, Layer, ObjectPool, BlendModes */

(function () {
    "use strict";
    
    paper.install(window);
    
    
    var config = {
        BACKGROUND_COLOR: "#eee",
        
        BOUNDS_PADDING: 5,

        TILE_COUNT: 500,
        
        BASE_SIZE: 50,
        BLEND_MODE: BlendModes.SOFT_LIGHT,
        STROKE_WIDTH: 0.5,
        STROKE_COLOR: "#333333",
        ROTATION_RANGE:3,
        
        CANVAS_WIDTH: 768,
        CANVAS_HEIGHT: 432, //16:9 aspect ratio
        SCALE_CANVAS: false,
        USE_RANDOM_COLORS: true,
        colorTheme: ColorTheme.themes.CROSSWALK
    };
    
    /*********** Override Config defaults here ******************/

    config.STROKE_WIDTH = 0.2;
    config.BASE_SIZE = 50;
    config.TILE_COUNT = 500;
    config.BACKGROUND_COLOR = "#eee"
    config.BLEND_MODE = BlendModes.SOFT_LIGHT;
    

    config.colorTheme = BRIGHTON_SHOES;
    
    /*************** End Config Override **********************/
    
    var colorTheme = new ColorTheme(config.colorTheme);
    var circleGroups = {};
    var t; //paperjs tool reference
    var circlesStore;
    var pixelData;
    
    var backgroundLayer;
    var tileLayer;
    
    var fileNameSuffix = new Date().getTime();
    
    var getColor = function () {
        
        var color;
        if (config.USE_RANDOM_COLORS) {
            color = colorTheme.getRandomColor();
        } else {
            color = colorTheme.getNextColor();
        }
        
        return color;
    };
    
    var getRandomPointInView = function () {
        
        var xPadding = config.BOUNDS_PADDING;
        var yPadding = config.BOUNDS_PADDING;
        
        var point = new Point(
            Math.floor(Math.random() * view.bounds.width),
            Math.floor(Math.random() * view.bounds.height)
        );
        
        if (xPadding || yPadding) {
            if (point.x < xPadding) {
                point.x = xPadding;
            } else if (point.x > view.bounds.width - xPadding) {
                point.x = view.bounds.width - xPadding;
            }
            
            if (point.y < yPadding) {
                point.y = yPadding;
            } else if (point.y > view.bounds.height - yPadding) {
                point.y = view.bounds.height - yPadding;
            }
        }
        
        return point;
    };
 
    //we could see if the sting is base 64 encoded, if not, assume its is a string
    //http://stackoverflow.com/a/5100158
    var dataURItoBlob = function (dataURI) {
        // convert base64 to raw binary data held in a string
        // doesn't handle URLEncoded DataURIs
        var byteString = atob(dataURI.split(',')[1]);

        // separate out the mime component
        var mimeString = dataURI.split(',')[0].split(':')[1].split(';')[0];

        // write the bytes of the string to an ArrayBuffer
        var ab = new ArrayBuffer(byteString.length);
        var ia = new Uint8Array(ab);
        
        var len = byteString.length;
        
        var i;
        for (i = 0; i < len; i++) {
            ia[i] = byteString.charCodeAt(i);
        }

        // write the ArrayBuffer to a blob, and you're done
        var bb = new Blob([ab], {type: mimeString});
        return bb;
    };
    
    var createName = function (extension) {
        return "tiles_example_" + fileNameSuffix + "." + extension;
    };
    
    var downloadFile = function (url, fileName) {

        var bb = dataURItoBlob(url);
        
        window.URL = window.URL || window.webkitURL;
        
        //http://html5-demos.appspot.com/static/a.download.html
        //https://developer.mozilla.org/en-US/docs/Web/API/Blob
        var a = document.createElement('a');
        a.download = fileName;
        a.href = window.URL.createObjectURL(bb);
        a.click();
    };
        
    var downloadAsPng = function () {
        var fileName = createName("png");
        
        var canvas = document.getElementById("myCanvas");
        var url = canvas.toDataURL("image/png");
        downloadFile(url, fileName);
    };
   
    var downloadConfig = function () {
        var fileName = createName("json");
        var url = "data:application/json;utf8," + btoa(JSON.stringify(config, null, "\t"));
        downloadFile(url, fileName);
    };
    
    var downloadAsSVG = function () {

        var fileName = createName("svg");
        
        var url = "data:image/svg+xml;utf8," + btoa(paper.project.exportSVG({asString: true}));
        downloadFile(url, fileName);
    };
    
    var initCanvas = function () {
        var drawCanvas = document.getElementById("myCanvas");
        var canvasW = config.CANVAS_WIDTH;
        var canvasH = config.CANVAS_HEIGHT;
        
        if (config.SCALE_CANVAS) {
            var maxW = window.innerWidth;
            var maxH = window.innerHeight;

            //http://www.ajaxblender.com/howto-resize-image-proportionally-using-javascript.html
            if (canvasH > maxH ||
                    canvasW > maxW) {

                var ratio = canvasH / canvasW;

                if (canvasW >= maxW && ratio <= 1) {
                    canvasW = maxW;
                    canvasH = canvasW * ratio;
                } else if (canvasH >= maxH) {
                    canvasH = maxH;
                    canvasW = canvasH / ratio;
                }
            }
        }
        
        drawCanvas.height = canvasH;
        drawCanvas.width = canvasW;
        
        return drawCanvas;
    };
    
    var getRandomSize = function () {
        var s = new Size(config.BASE_SIZE - 5, config.BASE_SIZE);
        s = s.multiply(Size.random().add(0.75)).round();

        return  s;
    };
    
    var getRandomRotation = function () {
        
        var a = config.ROTATION_RANGE;
        var r = ((Math.random() * a) - a) + (Math.random() * a);
        
        return r;
    }
    
    var getPointInBounds = function (size) {
        var point = getRandomPointInView();
        var isValid = false;
        while(!isValid) {

            if((point.x + size.width + config.BOUNDS_PADDING < view.bounds.width) &&
              (point.y + size.height + config.BOUNDS_PADDING < view.bounds.height)) {
                isValid = true;
            } else {
                point = getRandomPointInView();
            }
        }
        
        return point;
    }
    
    var generateTiles = function () {
        
        var out = [];
        var i;
        var rect;
        var point;
        var size;
        for (i = 0; i < config.TILE_COUNT; i++) {
            
            size = getRandomSize();
            point = getPointInBounds(size);
            
            rect = new Path.Rectangle({
                point: point,
                size: size,
                fillColor: getColor(),
                strokeColor: config.STROKE_COLOR,
                strokeWidth: config.STROKE_WIDTH,
                blendMode: config.BLEND_MODE,
                rotation:getRandomRotation()
            });
            
            out.push(out);
        }
        
        return out;
    };
    
    
    var tiles;
    window.onload = function () {

        var drawCanvas = initCanvas();
        
        paper.setup(drawCanvas);
        
        var backgroundLayer = project.activeLayer;

        //programtically set the background colors so we can set it once in a var.
        document.body.style.background = config.BACKGROUND_COLOR;
        drawCanvas.style.background = config.BACKGROUND_COLOR;
        
        var rect = new Path.Rectangle(new Point(0, 0),
                            new Size(view.bounds.width, view.bounds.height)
                );
        
        rect.fillColor = config.BACKGROUND_COLOR;

        tileLayer = new Layer();
        tiles = generateTiles();
        
        
        view.update();

        t = new Tool();

        //Listen for SHIFT-p to save content as SVG file.
        //Listen for SHIFT-o to save as PNG
        t.onKeyUp = function (event) {
            if (event.character === "S") {
                downloadAsSVG();
            } else if (event.character === "P") {
                downloadAsPng();
            } else if (event.character === "J") {
                downloadConfig();
            }
        };

    };
}());