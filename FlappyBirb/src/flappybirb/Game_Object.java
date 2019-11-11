package flappybirb;

import javafx.scene.canvas.GraphicsContext;

public class Game_Object {

    protected int x0, x1, y0, y1;
    protected double Initial_Velocity;
    protected double velocity;

    public Game_Object(int x0, int x1, int y0, int y1, double initVel) {
        this.x0 = x0;
        this.x1 = x1;
        this.y0 = y0;
        this.y1 = y1;
        this.Initial_Velocity = initVel;
        this.velocity = initVel;
    }

    public void move() {
    }

    public void unDraw(GraphicsContext gc) {
    }

    public void draw(GraphicsContext gc) {
    }
}
