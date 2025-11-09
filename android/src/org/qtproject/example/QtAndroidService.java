package org.qtproject.example;

import android.content.Context;
import android.content.Intent;
import android.util.Log;
import org.qtproject.qt.android.bindings.QtService;


public class QtAndroidService extends QtService
{
    private static final String TAG = "QtAndroidService";
    public static QtAndroidService INSTANCE;

    @Override
    public void onCreate() {
        super.onCreate();
        Log.i(TAG, "Creating Service");
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.i(TAG, "Destroying Service");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        int ret = super.onStartCommand(intent, flags, startId);

        // Do some work

        return ret;
    }

//    public static void startQtAndroidService(Context context)
//    {
//        context.startService(new Intent(context, QtAndroidService.class));
//        Log.d("TEST", "Android Service Start");
//    }
}
