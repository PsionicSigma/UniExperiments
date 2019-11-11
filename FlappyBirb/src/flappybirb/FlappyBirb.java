package flappybirb;

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

    private Birb Player = new Birb(130, 170, 230, 270, -5);
    private ArrayList<Pipe> Pipes = new ArrayList();
    private boolean Space_Pressed = false;
    private int Score_Counter = 0;
    private int Pipe_Velocity = 5;
    private int Pipe_Gap = 80;
    private int Last_Pipe_Gap_Y = 250;
    private int Peace_Period = 0;

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage GameStage) {

        GameStage.setTitle("FlappyBirb");
        Group RootGroup = new Group();
        Canvas GameCanvas = new Canvas(300, 500);
        RootGroup.getChildren().add(GameCanvas);
        Scene GameScene = new Scene(RootGroup);
        GameScene.setOnKeyPressed(event -> {
            if (event.getCode() == KeyCode.SPACE) {
                Player.flap();
            }
        });
        GameStage.setScene(GameScene);

        GraphicsContext GContext = GameCanvas.getGraphicsContext2D();

        GContext.setStroke(Color.BLACK);
        GContext.setLineWidth(1);
        Font theFont = Font.font("Calibri", FontWeight.BOLD, 50);
        GContext.setFont(theFont);

        new AnimationTimer() {
            public void handle(long currentNanoTime) {

                UpdateAll(GContext, currentNanoTime);
                GContext.setFill(Color.WHITE);
                GContext.fillText(Integer.toString(Score_Counter), 140, 75);
                GContext.strokeText(Integer.toString(Score_Counter), 140, 75);
                if (checkForGameEnd()) {
                    stop();
                    GContext.setFill(Color.WHITE);
                    GContext.fillText("Game ended", 20, 150);
                    GContext.strokeText("Game ended", 20, 150);
                }

            }
        }.start();
        GameStage.show();
    }

    public boolean checkForGameEnd() {
        for (Pipe p : Pipes) {
            if (Player.collides_With(p)) {
                return true;
            }
        }
        if (Player.getY0() < 0 || Player.getY1() > 500) {
            return true;
        }
        return false;
    }

    public void UpdateAll(GraphicsContext GC, long nanoTime) {
        long TempNanoTimer = System.nanoTime();
        Player.unDraw(GC);
        Player.move();
        Player.draw(GC);
        for (Pipe p : Pipes) {
            p.unDraw(GC);
            p.move();
            if (p.getX1() > 0) {
                p.draw(GC);
            }
        }
        if (GetPipeAvailability() && Peace_Period >= 100) {
            AddPipes();
        } else {
            Peace_Period++;
        }
        if (Score_Counter % 10 == 0 && (TempNanoTimer - nanoTime) / 1000000000 > 1) {
            AccelerateGame();
        }
    }

    public void AccelerateGame() {
        Pipe_Velocity += 30;
        for (Pipe p : Pipes) {
            p.accelerate();
        }
        Pipe_Gap -= 20;
    }

    public void AddPipes() {
        Pipes.add(new Pipe(300, 300 + 60, 0, (Last_Pipe_Gap_Y - Pipe_Gap), Pipe_Velocity));
        Pipes.add(new Pipe(300, 300 + 60, (Last_Pipe_Gap_Y + Pipe_Gap), 500, Pipe_Velocity));
        Last_Pipe_Gap_Y += (int) (Math.random() * (80) - 40);
        if (Last_Pipe_Gap_Y - Pipe_Gap < 20) {
            Last_Pipe_Gap_Y = Pipe_Gap + 20;
        }
        if (Last_Pipe_Gap_Y + Pipe_Gap > 480) {
            Last_Pipe_Gap_Y = 480 - Pipe_Gap;
        }
    }

    public boolean GetPipeAvailability() {
        int Last_Pipe_X = 0;
        for (Pipe p : Pipes) {
            if (Last_Pipe_X < p.getX1()) {
                Last_Pipe_X = p.getX1();
            }
        }
        if (Last_Pipe_X < 150 && Peace_Period >= 100) {
            Score_Counter++;
            return true;
        } else {
            return false;
        }
    }

}
