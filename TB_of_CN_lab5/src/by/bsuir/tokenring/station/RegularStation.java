package by.bsuir.tokenring.station;

import by.bsuir.tokenring.generator.NumberGenerator;

public class RegularStation extends Thread implements Station {
    private String data;
    private String markerBuffer;
    private int id;
    private boolean hasDataToSend;
    private int address;
    private boolean running;
    private String marker;

    {
        this.running = true;
        this.hasDataToSend = false;
    }

    public int getAddress() {
        return id + 48;
    }

    public RegularStation(String data, String marker, int id) {
        this.data = data;
        this.marker = marker;
        this.id = id;
        this.address = NumberGenerator.getRandomNumber(6, 1, id);
    }

    public void setMarkerBuffer(String markerBuffer) {
        this.markerBuffer = markerBuffer;
    }

    @Override
    public boolean hasDataToSend() {
        return hasDataToSend;
    }

    public String showExternalData(String dataToShow) {
        System.out.println("Address: " + id + "\nExternal data: " + dataToShow);
        if(dataToShow.charAt(0) != '0') {
            dataToShow += '0';
        }
        return dataToShow;
    }

    @Override
    public void run() {
        while(running) {
            waitSomeTime(100);
            if(NumberGenerator.getRandomNumber(1,0, 0) > 0) {
                hasDataToSend = true;
            } else {
                hasDataToSend = false;
                waitSomeTime(NumberGenerator.getRandomNumber(3000,500, 0));
            }
        }
    }

    public String prepareData(String monitorMarker) {
        setMarkerBuffer(monitorMarker);
        return marker + data + address + id;
    }

    public String getOldMarker() {
        return this.markerBuffer;
    }

    private void waitSomeTime(long time) {
        try {
            Thread.sleep(time);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void closeThread() {
        this.running = false;
    }
}
