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
        INSTANCE = this;
        Log.i(TAG, "Creating Service");
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        INSTANCE = null;
        Log.i(TAG, "Destroying Service");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        int ret = super.onStartCommand(intent, flags, startId);
        return android.app.Service.START_STICKY;
    }

    public static void startQtAndroidService(Context context)
    {
        if (context != null) {
            Intent serviceIntent = new Intent(context, QtAndroidService.class);
            context.startService(serviceIntent);
            Log.d(TAG, "Android Service Start");
        }
    }

    public static void stopQtAndroidService(Context context)
    {
        if (context != null) {
            Intent serviceIntent = new Intent(context, QtAndroidService.class);
            context.stopService(serviceIntent);
            Log.d(TAG, "Android Service Stop");
        }
    }
}
