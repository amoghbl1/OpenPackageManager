package org.opm.openpackagemanager;

import android.content.Context;
import android.content.res.Resources;
import android.util.Log;
import android.widget.Toast;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * Created by amoghbl1 on 18/03/16.
 */
public class BinaryInstaller {
    Context context;

    String DEBUG_TAG = "myTag";

    public BinaryInstaller (Context context) {
        this.context = context;
    }
    /*

     */
    public boolean installResources() {
        InputStream inputStream;
        File outFile;
        File appBinHome = context.getExternalFilesDir(null);
        File oldAppBin = context.getDir("bin", Context.MODE_MULTI_PROCESS);
        Resources resources = context.getResources();

        try {
            // Delete the file before we write anything there
            CommandRunner.execCommand("rm -rf ./", appBinHome);

            inputStream = resources.openRawResource(R.raw.nmap);
            outFile = new File(appBinHome, "nmap");
            moveBinaryRawResourceToFile(inputStream, outFile);

            inputStream = resources.openRawResource(R.raw.busybox);
            outFile = new File(oldAppBin, "busybox");
            moveBinaryRawResourceToFile(inputStream, outFile);

            String []binaries = {"nmap", "busybox"};
            String output;

            // Changing all the permissions of the files in the app_bin folder
            for(int i=0; i<binaries.length ; i++) {
                output = CommandRunner.execCommand("chmod 6755 ./" + binaries[i], oldAppBin.getAbsoluteFile());
                Log.d(DEBUG_TAG, "App Bin Home "+appBinHome.getAbsolutePath());
                Log.d(DEBUG_TAG, "Old Bin Home "+oldAppBin.getAbsolutePath());
                output = CommandRunner.execCommand("ln -s " + oldAppBin.getAbsolutePath() + "/" + binaries[i] + " " + appBinHome.getAbsolutePath() + "/newNmap", oldAppBin.getAbsoluteFile());
                Log.d(DEBUG_TAG, "New output: " + output);
                Log.d(DEBUG_TAG, "chmod output: " + CommandRunner.execCommand("ls -la ./" + binaries[i], appBinHome.getAbsoluteFile()));
            }
        }
        catch (IOException e) {
            Toast.makeText(context, "IOException!", Toast.LENGTH_LONG).show();
            Log.d(DEBUG_TAG, e.getMessage());
        }
        catch (InterruptedException e) {
            Toast.makeText(context, "Command execution Interrupted!", Toast.LENGTH_LONG).show();
            Log.d(DEBUG_TAG, "Interrupted Exception: " + e.getMessage());
        }

        return true;
    }

    /*
    Write from an input stream to an output file
    */
    private void moveBinaryRawResourceToFile(InputStream inputStream, File outFile) throws IOException {
        byte[] buf = new byte[1024];
        int bytecount;
        OutputStream outputStream = new FileOutputStream(outFile.getAbsolutePath());
        while((bytecount = inputStream.read(buf)) > 0) {
            outputStream.write(buf, 0, bytecount);
        }
        inputStream.close();
        outputStream.close();
    }
}