package by.bsuir.tokenring;

import by.bsuir.tokenring.compare.DataComparator;
import by.bsuir.tokenring.station.Station;
import by.bsuir.tokenring.station.RegularStation;

import java.util.ArrayList;
import java.util.List;

public class Main {
    private static int CYCLE_AMOUNT = 20;
    private static int MONITOR_STATION_NUMBER = 1;

    public static void main(String[] args) {
        RegularStation one = new RegularStation("Station1", "1marker1",1);
        RegularStation two = new RegularStation("Station2", "2marker2",2);
        RegularStation three = new RegularStation("Station3", "3marker3",3);
        RegularStation four = new RegularStation("Station4", "4marker4",4);
        RegularStation five = new RegularStation("Station5", "5marker5",5);
        RegularStation six = new RegularStation("Station6", "6marker6",6);

        Thread first = new Thread(one);
        Thread second = new Thread(two);
        Thread third = new Thread(three);
        Thread fourth = new Thread(four);
        Thread fifth = new Thread(five);
        Thread sixth = new Thread(six);

        List<Station> stations = new ArrayList<>();
        stations.add(one);
        stations.add(two);
        stations.add(three);
        stations.add(four);
        stations.add(five);
        stations.add(six);

        String marker = MONITOR_STATION_NUMBER + "-M";
        boolean isData = false;

        first.start();
        second.start();
        third.start();
        fourth.start();
        fifth.start();
        sixth.start();

        while(CYCLE_AMOUNT > 0) {
            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            for (Station station:stations) {

                if(station.hasDataToSend() && !isData) {
                    marker = station.prepareData(marker);
                    isData = true;
                }

                if(isData) {
                    if(marker.charAt(marker.length()-2) == station.getAddress()
                            && marker.charAt(marker.length()-1) != '0') {
                        System.out.println("DESTINATION:");
                        marker = station.showExternalData(marker);
                    }
                    if(marker.charAt(0) == station.getAddress() && marker.charAt(marker.length()-1) == '0') {
                        marker = station.getOldMarker();
                        isData = false;
                    }
                }
            }
           CYCLE_AMOUNT--;
        }

        for (Station station : stations) {
            station.closeThread();
        }
    }
}
