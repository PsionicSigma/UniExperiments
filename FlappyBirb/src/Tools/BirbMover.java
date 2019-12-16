package Tools;

import Tools.Mover;

public class BirbMover implements Mover {

    @Override
    public int move(double velocity, int x0, int y0, int x1, int y1) {
        return y0 + (int) velocity;
    }

}
