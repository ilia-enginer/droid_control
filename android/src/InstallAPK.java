
package android.src.InstallAPK;
import org.qtproject.qt.android.QtNative;
import java.lang.String;
import java.io.File;
import android.content.Intent;
import android.util.Log;
import android.net.Uri;
import android.content.ContentValues;
import android.content.Context;
import android.support.v4.content.FileProvider;


public class InstallAPK
{
    protected InstallAPK()
    {
    }

    private  Context context = this.context;

    public  int installApp(String appPackageName) {

         if (QtNative.activity() == null)
            return -1;
         try {
             File apk = new File(appPackageName);
             Intent intent = new Intent(Intent.ACTION_VIEW);
             intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK | Intent.FLAG_ACTIVITY_NEW_TASK);
             intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
             Uri uri = FileProvider.getUriForFile(context,"org.qtproject.install.fileProvider", apk);
             intent.setDataAndType(uri,"application/vnd.android.package-archive");

             QtNative.activity().startActivity(intent);

             return 0;
         } catch (android.content.ActivityNotFoundException anfe) {
             return -3;
         }
     }
}

