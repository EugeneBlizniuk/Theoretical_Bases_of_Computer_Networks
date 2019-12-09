package by.bsuir.tokenring.generator;

public class NumberGenerator {
    public static int getRandomNumber(int max, int min, int exception) {
        int randomNumber = exception;
        while(randomNumber == exception) {
            randomNumber = (int)(Math.random()* (max - min + 1.0) + min);
        }
        return randomNumber;
    }

//    public static void waitSomeTime(long time) {
//        try {
//            Thread.sleep(time);
//        } catch (InterruptedException e) {
//            e.printStackTrace();
//        }
//    }
}