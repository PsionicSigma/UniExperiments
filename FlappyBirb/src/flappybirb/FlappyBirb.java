package flappybirb;
import Tools.*;

import java.util.ArrayList;
import javafx.animation.AnimationTimer;
import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.input.KeyCode;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.stage.Stage;

public class FlappyBirb extends Application {

    private Birb player = Birb.getInstance();
    private ArrayList<Pipe> Pipes = new ArrayList();
    private boolean spacePressed = false;
    private boolean running = false;
    private boolean canAccelerate = false;
    private int score = 0;
    private int pipeVelocity = 5;
    private int pipeGap = 80;
    private int lastPipeGapY = 250;
    private int peacePeriod = 0;

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage gameStage) {

        gameStage.setTitle("FlappyBirb");

        Group rootGroup = new Group();

        Canvas gameCanvas = new Canvas(HardDataContainer.canvasWidth, HardDataContainer.canvasHeight);

        GraphicsContext gc = gameCanvas.getGraphicsContext2D();

        gc.setStroke(Color.BLACK);
        gc.setLineWidth(1);
        Font theFont = Font.font("Calibri", FontWeight.BOLD, 50);
        gc.setFont(theFont);

        rootGroup.getChildren().add(gameCanvas);

        Scene gameScene = new Scene(rootGroup);

        gameScene.setOnKeyPressed(event -> {
            if (event.getCode() == KeyCode.SPACE && running) {
                if (!spacePressed) {
                    player.flap();
                    spacePressed = true;
                }
            } else if (event.getCode() == KeyCode.SPACE && !running) {
                gc.setStroke(Color.WHITE);
                gc.setFill(Color.WHITE);
                gc.fillText("Game ended", 20, 150);
                gc.strokeText("Press SPACE\n      to flap", 10, 150);
                gc.setStroke(Color.BLACK);
                running = true;
            }
        });
        gameScene.setOnKeyReleased(event -> {
            if (event.getCode() == KeyCode.SPACE) {
                spacePressed = false;
            }
        });

        gameStage.setScene(gameScene);

        player.draw(gc);
        gc.setFill(Color.WHITE);
        gc.fillText("Press SPACE\n      to flap", 10, 150);
        gc.strokeText("Press SPACE\n      to flap", 10, 150);
        new AnimationTimer() {
            public void handle(long currentNanoTime) {
                if (running) {
                    updateAllObjects(gc, currentNanoTime);
                    gc.setFill(Color.WHITE);
                    gc.fillText(Integer.toString(score), 140, 75);
                    gc.strokeText(Integer.toString(score), 140, 75);
                    if (checkForGameEnd()) {
                        stop();
                        gc.setFill(Color.WHITE);
                        gc.fillText("Game ended", 20, 150);
                        gc.strokeText("Game ended", 20, 150);
                    }
                }
            }
        }.start();

        gameStage.show();
    }

    public boolean checkForGameEnd() {
        for (Pipe p : Pipes) {
            if (player.collidesWith(p)) {
                return true;
            }
        }
        if (player.getY0() < 0 || player.getY1() > HardDataContainer.canvasHeight) {
            return true;
        }
        return false;
    }

    public void updateAllObjects(GraphicsContext gc, long nanoTime) {
        long tempNanoTimer = System.nanoTime();
        player.unDraw(gc);
        player.move();
        player.draw(gc);
        for (Pipe p : Pipes) {
            p.unDraw(gc);
            p.move();
            if (p.getX1() > 0) {
                p.draw(gc);
            }
        }
        if (getPipeAvailability() && peacePeriod >= HardDataContainer.peaceTime) {
            addPipes();
        } else {
            peacePeriod++;
        }
        if (score % 10 == 0) {
            if (canAccelerate) {
                accelerateGame();
                canAccelerate = false;
            }
        } else {
            canAccelerate = true;
        }
    }

    public void accelerateGame() {
        pipeVelocity += HardDataContainer.pipeAcceleration;
        for (Pipe p : Pipes) {
            p.accelerate();
        }
        if (pipeGap > 0 + pipeGap / 2) {
            pipeGap -= HardDataContainer.gapDecrease;
        }
    }

    public void addPipes() {
        Pipes.add(new Pipe(HardDataContainer.pipeStartX0, HardDataContainer.pipeStartX1, HardDataContainer.pipeStartY0Top, (lastPipeGapY - pipeGap), pipeVelocity));
        Pipes.add(new Pipe(HardDataContainer.pipeStartX0, HardDataContainer.pipeStartX1, (lastPipeGapY + pipeGap), HardDataContainer.pipeStartY1Bottom, pipeVelocity));
        lastPipeGapY += (int) (Math.random() * (pipeGap) - pipeGap / 2);
        if (lastPipeGapY - pipeGap < HardDataContainer.topPipeGapBorder) {
            lastPipeGapY = pipeGap + HardDataContainer.topPipeGapBorder;
        }
        if (lastPipeGapY + pipeGap > HardDataContainer.bottomPipeGapBorder) {
            lastPipeGapY = HardDataContainer.bottomPipeGapBorder - pipeGap;
        }
    }

    public boolean getPipeAvailability() {
        int lastPipeX = 0;
        for (Pipe p : Pipes) {
            if (lastPipeX < p.getX1()) {
                lastPipeX = p.getX1();
            }
        }
        if (lastPipeX < HardDataContainer.canvasWidth / 2 && peacePeriod >= HardDataContainer.peaceTime) {
            if (!Pipes.isEmpty()) {
                score++;
            }
            return true;
        } else {
            return false;
        }
    }
}
