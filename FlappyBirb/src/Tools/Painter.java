package Tools;

import javafx.scene.canvas.GraphicsContext;

public interface Painter {

    void draw(GraphicsContext gc, int x0, int y0, int x1, int y1);

    void unDraw(GraphicsContext gc, int x0, int y0, int x1, int y1);

}
