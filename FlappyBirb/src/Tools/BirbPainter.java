package Tools;

import Tools.Painter;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;

public class BirbPainter implements Painter {

    @Override
    public void draw(GraphicsContext gc, int x0, int y0, int x1, int y1) {
        gc.setFill(Color.YELLOW);
        gc.fillRect(x0, y0, x1 - x0, y1 - y0);
    }

    @Override
    public void unDraw(GraphicsContext gc, int x0, int y0, int x1, int y1) {
        gc.setFill(Color.WHITE);
        gc.fillRect(x0, y0, x1 - x0, y1 - y0);
    }

}
