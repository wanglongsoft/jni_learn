package soft.wl.ndkdemo;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Parcel;
import android.util.Log;

import soft.wl.control.FunctionControl;

public class MainActivity extends AppCompatActivity {

    private final String TAG = "NDKDemoTest";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        FunctionControl functionControl = new FunctionControl();
        Parcel in = Parcel.obtain();
        Parcel out = Parcel.obtain();
        functionControl.sendCommand(1024, in, out);
        Log.d(TAG, "onCreate readInt After One : " + in.readInt());
        Log.d(TAG, "onCreate readInt After Two : " + in.readInt());
        in.recycle();
        out.recycle();
    }
}
