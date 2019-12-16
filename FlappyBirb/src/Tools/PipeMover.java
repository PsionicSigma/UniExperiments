package Tools;

public class PipeMover implements Mover {

    @Override
    public int move(double velocity, int x0, int y0, int x1, int y1) {
        return x0 - (int) velocity;
    }

}
