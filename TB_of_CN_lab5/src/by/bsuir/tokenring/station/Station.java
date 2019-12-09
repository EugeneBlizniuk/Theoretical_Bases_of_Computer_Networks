package by.bsuir.tokenring.station;

public interface Station {
    public boolean hasDataToSend();
    public String prepareData(String monitorMarker);
    public int getAddress();
    public String showExternalData(String dataToShow);
    public String getOldMarker();
    public void closeThread();
}
