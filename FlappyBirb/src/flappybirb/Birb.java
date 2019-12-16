package flappybirb;
import Tools.*;

public class Birb extends GameObject implements CollisionChecker<Pipe> {

    private static Birb player = new Birb(HardDataContainer.birbStartX0, HardDataContainer.birbStartX1, HardDataContainer.birbStartY0, HardDataContainer.birbStartY1, HardDataContainer.birbInitVel);

    private Birb(int x0, int x1, int y0, int y1, double initVel) {
        super(x0, x1, y0, y1, initVel, new BirbPainter(), new BirbMover());
    }

    public static Birb getInstance() {
        return player;
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
        return initialVelocity;
    }

    public void setInitial_Velocity(int Initial_Velocity) {
        this.initialVelocity = Initial_Velocity;
    }

    public double getVelocity() {
        return velocity;
    }

    public void setVelocity(int velocity) {
        this.velocity = velocity;
    }

    public void flap() {
        velocity = initialVelocity;
    }

    @Override
    public void move() {
        y0 = moveBehavior.move(velocity, x0, y0, x1, y1);
        y1 = y0 + HardDataContainer.birbHeight;
        velocity = velocity + 0.2;
    }

    @Override
    public boolean collidesWith(Pipe Target_Pipe) {
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
