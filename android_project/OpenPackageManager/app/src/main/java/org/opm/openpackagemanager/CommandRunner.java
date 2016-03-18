package org.opm.openpackagemanager;

import android.util.Log;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * Created by amoghbl1 on 18/03/16.
 */

public class CommandRunner {

    private String DEBUG_TAG = "myTag";

    public static String execCommand(String command, File currentDirectory) throws IOException, InterruptedException{
        Process process = Runtime.getRuntime().exec(command, null, currentDirectory);
        process.waitFor();
        BufferedReader readerInputStream = new BufferedReader(
                new InputStreamReader(process.getInputStream()));
        BufferedReader readerErrorStream = new BufferedReader(
                new InputStreamReader(process.getErrorStream())
        );
        int read;
        char[] buf = new char[4096];
        StringBuffer output = new StringBuffer();
        StringBuffer error = new StringBuffer();
        while ((read = readerInputStream.read(buf)) > 0) {
            output.append(buf, 0, read);
        }
        while((read = readerErrorStream.read(buf)) > 0) {
            error.append(buf, 0, read);
        }
        readerInputStream.close();
        readerErrorStream.close();
        Log.d("myTag", error.toString());
        return output.toString() + error.toString();
    }
}
