package flappybirb;
import Tools.*;

import javafx.scene.canvas.GraphicsContext;

public class GameObject {

    protected int x0, x1, y0, y1;
    protected double initialVelocity;
    protected double velocity;

    protected Painter paintBehavior;
    protected Mover moveBehavior;

    public GameObject(int x0, int x1, int y0, int y1, double initVel, Painter painter, Mover mover) {
        this.x0 = x0;
        this.x1 = x1;
        this.y0 = y0;
        this.y1 = y1;
        this.initialVelocity = initVel;
        this.velocity = initVel;

        this.paintBehavior = painter;
        this.moveBehavior = mover;
    }

    public void move() {

    }

    public void unDraw(GraphicsContext gc) {
        paintBehavior.unDraw(gc, x0, y0, x1, y1);
    }

    public void draw(GraphicsContext gc) {
        paintBehavior.draw(gc, x0, y0, x1, y1);
    }
}
