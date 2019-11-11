package flappybirb;

import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;

public class Birb extends Game_Object implements Collideable<Pipe> {

    public Birb(int x0, int x1, int y0, int y1, int initVel) {
        super(x0, x1, y0, y1, initVel);
    }

    public int getX0() {
        return x0;
    }

    public void setX0(int x0) {
        this.x0 = x0;
    }

    public int getX1() {
        return x1;
    }

    public void setX1(int x1) {
        this.x1 = x1;
    }

    public int getY0() {
        return y0;
    }

    public void setY0(int y0) {
        this.y0 = y0;
    }

    public int getY1() {
        return y1;
    }

    public void setY1(int y1) {
        this.y1 = y1;
    }

    public double getInitial_Velocity() {
        return Initial_Velocity;
    }

    public void setInitial_Velocity(int Initial_Velocity) {
        this.Initial_Velocity = Initial_Velocity;
    }

    public double getVelocity() {
        return velocity;
    }

    public void setVelocity(int velocity) {
        this.velocity = velocity;
    }

    public void flap() {
        velocity = Initial_Velocity;
    }

    @Override
    public void draw(GraphicsContext gc) {
        gc.setFill(Color.YELLOW);
        gc.fillRect(x0, y0, x1 - x0, y1 - y0);
    }

    @Override
    public void unDraw(GraphicsContext gc) {
        gc.setFill(Color.WHITE);
        gc.fillRect(x0, y0, x1 - x0, y1 - y0);
    }

    @Override
    public void move() {
        y0 = y0 + (int) velocity;
        y1 = y0 + 40;
        velocity = velocity + 0.2;
    }

    @Override
    public boolean collides_With(Pipe Target_Pipe) {
        if ((this.x0 > Target_Pipe.x0 && this.x0 < Target_Pipe.x1) && (this.y0 > Target_Pipe.y0 && this.y0 < Target_Pipe.y1)) {
            return true;
        }
        if ((this.x0 > Target_Pipe.x0 && this.x0 < Target_Pipe.x1) && (this.y1 > Target_Pipe.y0 && this.y1 < Target_Pipe.y1)) {
            return true;
        }
        if ((this.x1 > Target_Pipe.x0 && this.x1 < Target_Pipe.x1) && (this.y0 > Target_Pipe.y0 && this.y0 < Target_Pipe.y1)) {
            return true;
        }
        if ((this.x1 > Target_Pipe.x0 && this.x1 < Target_Pipe.x1) && (this.y1 > Target_Pipe.y0 && this.y1 < Target_Pipe.y1)) {
            return true;
        }
        return false;
    }

}
