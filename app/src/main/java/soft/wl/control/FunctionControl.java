package soft.wl.control;

import android.os.Parcel;

public class FunctionControl {
    static {
        System.loadLibrary("FunctionControl");
    }
    public FunctionControl() {
    }
    native public int sendCommand(int cmd , Parcel in, Parcel out);
}
