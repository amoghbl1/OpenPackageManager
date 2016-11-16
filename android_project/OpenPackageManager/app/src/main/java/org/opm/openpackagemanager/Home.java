package org.opm.openpackagemanager;


import android.app.ProgressDialog;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.io.File;
import java.io.IOException;

import static android.content.Context.MODE_MULTI_PROCESS;


public class Home extends Fragment {

    String DEBUG_TAG = "myTag";

    String DEFAULT_SHARED_PREFERENCES = "mySharedPrefs";
    String firstStartPref = "firstStart";

    public static File appBinHome, oldBinHome;

    public static TextView scanResult = null;
    public static SharedPreferences mySharedPreferences = null;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        boolean firstInstall = true;
        appBinHome = getActivity().getExternalFilesDir(null);
        oldBinHome = getActivity().getDir("bin", MODE_MULTI_PROCESS);

        mySharedPreferences = getActivity().getSharedPreferences(DEFAULT_SHARED_PREFERENCES, MODE_MULTI_PROCESS);
        firstInstall = mySharedPreferences.getBoolean(firstStartPref, true);
        if(firstInstall) {
            new AsyncBinaryInstaller().execute();
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View inflated = inflater.inflate(R.layout.activity_home, container, false);
        Button scan = (Button)inflated.findViewById(R.id.scan_BT);
        final EditText flags = (EditText)inflated.findViewById(R.id.flags_ET);
        scanResult = (TextView)inflated.findViewById(R.id.scan_output_TV);


        scan.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String f = flags.getText().toString();
                new AsyncCommandExecutor().execute(f);
            }
        });

        return inflated;

    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    public class AsyncBinaryInstaller extends AsyncTask<Void, Void, Void> {
        private ProgressDialog progressDialog = new ProgressDialog(getActivity());


        @Override
        protected void onPreExecute() {
            progressDialog.setTitle("Open Package Manager");
            progressDialog.setMessage("First Install, bootstrapping...");
            progressDialog.setCancelable(false);
            progressDialog.show();
            return;
        }

        @Override
        protected Void doInBackground(Void... params) {
            BinaryInstaller installer = new BinaryInstaller(getActivity());
            installer.installResources();
            Log.d(DEBUG_TAG, "Installing binaries");
            // TODO: Write some test code to see if the binaries are placed correctly and have the right permissions!
            Home.mySharedPreferences.edit().putBoolean(firstStartPref, false).commit();
            return null;
        }

        @Override
        protected void onPostExecute(Void result) {
            if(this.progressDialog.isShowing())
                this.progressDialog.dismiss();
        }
    }

    public class AsyncCommandExecutor extends AsyncTask<String, Void, Void> {

        public String returnOutput;
        private ProgressDialog progressDialog = new ProgressDialog(getActivity());

        @Override
        protected void onPreExecute() {
            this.progressDialog.setMessage("Running command.");
            this.progressDialog.setCancelable(false);
            this.progressDialog.show();
            return;
        }

        @Override
        protected Void doInBackground(String... params) {
            try {
                Log.d(DEBUG_TAG, "Running " + params[0]);
                if(!params[0].equals(""))
                    this.returnOutput = CommandRunner.execCommand(params[0], Home.oldBinHome.getAbsoluteFile());
            } catch (IOException e) {
                this.returnOutput = "IOException while running command";
                Log.d(DEBUG_TAG, e.getMessage());
            } catch (InterruptedException e) {
                this.returnOutput = "Command Interrupted!";
                Log.d(DEBUG_TAG, e.getMessage());
            }

            return null;
        }

        @Override
        protected void onPostExecute(Void result) {
            Home.scanResult.setText(returnOutput);
            if(this.progressDialog.isShowing())
                this.progressDialog.dismiss();
        }
    }
}
