void drawCircle(Point p, float radius) {
	ellipse(p.x, p.y, radius * 2, radius * 2);
}

void drawLine(Point p1, Point p2) {
	line(p1.x, p1.y, p2.x, p2.y);
}

Point getRandomPoint(int padding) {
	return new Point(random(padding, width - padding), random(padding, height - padding));
}

import java.awt.BasicStroke;
import java.awt.Graphics2D;
void strokeDash(float width) {

	BasicStroke pen;
	float[] dashes = { 2.0f};
	pen = new BasicStroke(width, BasicStroke.CAP_ROUND, BasicStroke.JOIN_MITER, 4.0f, dashes, 0.0f);

	Graphics2D g2 = ((PGraphicsJava2D) g).g2;
  	g2.setStroke(pen);
}

Point findPointOnQuadraticCurve(Point p1, Point p2, Point cp, float t) {
	//http://en.wikipedia.org/wiki/B%C3%A9zier_curve#Quadratic_B.C3.A9zier_curves
	float x = (1 - t) * (1 - t) * p1.x + 2 * (1 - t) * t * cp.x + t * t * p2.x;
	float y = (1 - t) * (1 - t) * p1.y + 2 * (1 - t) * t * cp.y + t * t * p2.y;

	return new Point(x,y);
}

void setBlendModeByName(String blendModeName) {
	int mode = BLEND;

	if(blendModeName == "REPLACE") {
		mode = REPLACE;
	} else if (blendModeName == "BLEND") {
		mode = BLEND;
	} else if (blendModeName == "ADD") {
		mode = ADD;
	} else if (blendModeName == "SUBTRACT") {
		mode = SUBTRACT;
	} else if (blendModeName == "LIGHTEST") {
		mode = LIGHTEST;
	}  else if (blendModeName == "DARKEST") {
		mode = DARKEST;
	} else if (blendModeName == "DIFFERENCE") {
		mode = DIFFERENCE;
	} else if (blendModeName == "EXCLUSION") {
		mode = EXCLUSION;
	} else if (blendModeName == "MULTIPLY") {
		mode = MULTIPLY;
	} else if (blendModeName == "SCREEN") {
		mode = SCREEN;
	}  else if (blendModeName == "OVERLAY") {
		mode = OVERLAY;
	} else if (blendModeName == "HARD_LIGHT") {
		mode = HARD_LIGHT;
	} else if (blendModeName == "SOFT_LIGHT") {
		mode = SOFT_LIGHT;
	} else if (blendModeName == "DODGE") {
		mode = DODGE;
	} else if (blendModeName == "BURN") {
		mode = BURN;
	}  else if (blendModeName == "NORMAL") {
		mode = BLEND;
	}

  blendMode(mode);
}


class QuadraticCurve {

	Point p1 = null;
	Point p2 = null;
	Point cp = null;
	QuadraticCurve (Point _p1, Point _p2, Point _cp) {
		p1 = _p1;
		p2 = _p2;
		cp = _cp;
	}

	QuadraticCurve () {

	}
}

class Size {
	float width = 0.0;
	float height = 0.0;

	Size(float width, float height) {
		this.width = width;
		this.height = height;
	}
}


