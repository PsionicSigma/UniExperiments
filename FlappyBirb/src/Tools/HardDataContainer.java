package Tools;

public abstract class HardDataContainer {

    //graphics
    public static int canvasWidth = 300;
    public static int canvasHeight = 500;
    //coordinates
    public static int birbWidth = 40;
    public static int birbHeight = 40;
    public static int birbStartX0 = 130;
    public static int birbStartX1 = birbStartX0 + birbWidth;
    public static int birbStartY0 = 230;
    public static int birbStartY1 = birbStartY0 + birbHeight;
    public static double birbInitVel = -5;
    public static int pipeWidth = 60;
    public static int pipeStartX0 = canvasWidth;
    public static int pipeStartX1 = pipeStartX0 + pipeWidth;
    public static int pipeStartY0Top = 0;
    public static int pipeStartY1Bottom = canvasHeight;
    public static int topPipeGapBorder = 40;
    public static int bottomPipeGapBorder = 460;
    public static int pipeAcceleration = 1;
    public static int gapDecrease = 10;
    //other
    public static int peaceTime = 100;
}
