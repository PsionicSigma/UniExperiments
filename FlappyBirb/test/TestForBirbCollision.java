import flappybirb.*;
import Tools.*;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

public class TestForBirbCollision {

    private Birb testSubjectBirb = Birb.getInstance();
    private Pipe testSubjectPipe = new Pipe(80, 140, 200, 500, 5);

    public TestForBirbCollision() {
    }

    @BeforeClass
    public static void setUpClass() {
    }

    @AfterClass
    public static void tearDownClass() {
    }

    @Before
    public void setUp() {
        testSubjectBirb.setX0(70);
        testSubjectBirb.setX1(110);
        testSubjectBirb.setY0(190);
        testSubjectBirb.setY0(230);
    }

    @After
    public void tearDown() {
    }

    @Test
    public void testBirbCollision() {
        setUp();
        Assert.assertTrue(testSubjectBirb.collidesWith(testSubjectPipe));
    }

    @Test
    public void testPipeAcceleration() {
        setUp();
        int baseVelocity = (int) testSubjectPipe.getVelocity();
        testSubjectPipe.accelerate();
        int higherVelocity = (int) testSubjectPipe.getVelocity();
        Assert.assertEquals(baseVelocity + HardDataContainer.pipeAcceleration, higherVelocity);
    }

    @Test
    public void testPipeMove() {
        setUp();
        int baseX = (int) testSubjectPipe.getX0();
        testSubjectPipe.move();
        int deltaX = (int) testSubjectPipe.getX0();
        Assert.assertEquals(baseX -5, deltaX);
    }
    
}
