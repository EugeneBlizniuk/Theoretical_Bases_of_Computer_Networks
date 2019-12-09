package by.bsuir.tokenring.compare;

public final class DataComparator {
    private DataComparator() {}
    private static DataComparator INSTANCE = new DataComparator();

    public static DataComparator getInstance() {
        return INSTANCE;
    }

    public boolean compareAddress(char first, char second) {
        return first == second;
    }
}
