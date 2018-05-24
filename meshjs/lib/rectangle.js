import Vector from "./vector.js"

export default class Rectangle {

	//todo: check if it should be h / w or w / h (canvas clear)
	constructor(x, y, width, height) {
		this._x = x;
		this._y = y;
		this._height = height;
		this._width = width;
		this._updateCenter();
	}

	_updateCenter(){

		if(!this._center) {
			this._center = new Vector();
		}

		this._center.x = this._width / 2;
		this._center.y = this._height / 2;
	}

	get center() {
		return this._center;
	}

	get height() {
		return this._height;
	}

	set height(height) {
		this._height = height;
		this._updateCenter();
	}

	get width(){
		return this._width;
	}

	set width(width){
		this._width = width;
		this._updateCenter();
	}

	get x(){
		return this._x;
	}

	set x(x) {
		this._x = x;
		this._updateCenter();
	}

	get y() {
		return this._y;
	}

	set y(y) {
		this._y = y;
		this._updateCenter();
	}

	get topLeft() {
		return new Vector(this._x, this._y);
	}

	get topRight() {
		return new Vector(this._width, this._y);
	}

	get bottomRight() {
		return new Vector(this._width, this._height);
	}

	get bottomLeft() {
		return new Vector(this._x, this._height);
	}

	//scales the rectangle by included amout. note, if you want to make a copy
	//first call clone : r.clone().scale(2);
	scale(scale) {
		this._height *= scale;
		this._width += scale;
	}

	//returns a new instance of Rectangle, with dimensions based on existing
	//instance with padding applied to all 4 sides
	withPadding(padding) {
		let r = this.clone();

		r.x += padding;
		r.width -= (padding * 2);

		r.y += padding;
		r.height -= (padding * 2);

		return r;
	}

	clone() {
		return new Rectangle(
			this._x,
			this._y,
			this._height,
			this._width
		);
	}
}
