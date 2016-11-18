package org.opm.openpackagemanager;

import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.widget.SwipeRefreshLayout;
import android.support.v7.widget.DefaultItemAnimator;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;


/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link ServerListFragment.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link ServerListFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class ServerListFragment extends Fragment {
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;
    public static View inflated;
    private RecyclerView recyclerView;
    static SwipeRefreshLayout mySwipeRefreshLayout;
    private PackageAdapter adapter;
    private List<Package> packageList;

    private OnFragmentInteractionListener mListener;

    public ServerListFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment ServerListFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static ServerListFragment newInstance(String param1, String param2) {
        ServerListFragment fragment = new ServerListFragment();
        Bundle args = new Bundle();
        args.putString(ARG_PARAM1, param1);
        args.putString(ARG_PARAM2, param2);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            mParam1 = getArguments().getString(ARG_PARAM1);
            mParam2 = getArguments().getString(ARG_PARAM2);
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment

        inflated = inflater.inflate(R.layout.fragment_server_list, container, false);

        recyclerView = (RecyclerView) inflated.findViewById(R.id.recycler_view_server);
        final TextView notpackages = (TextView)inflated.findViewById(R.id.not_found);
        packageList = new ArrayList<>();
        adapter = new PackageAdapter(inflated.getContext(), packageList);
        notpackages.setVisibility(View.GONE);
        mySwipeRefreshLayout = (SwipeRefreshLayout) inflated.findViewById(R.id.swiperefreshserver);
        mySwipeRefreshLayout.setRefreshing(true);
        LinearLayoutManager llm = new LinearLayoutManager(inflated.getContext());
        recyclerView.setLayoutManager(llm);
        recyclerView.setItemAnimator(new DefaultItemAnimator());
        recyclerView.setAdapter(adapter);
        mySwipeRefreshLayout.setOnRefreshListener(
                new SwipeRefreshLayout.OnRefreshListener() {
                    @Override
                    public void onRefresh() {
                        // This method performs the actual data-refresh operation.
                        // The method calls setRefreshing(false) when it's finished.
                        try {
                            notpackages.setVisibility(View.GONE);
                            prepareServerPackages();
                        } catch (FileNotFoundException e) {
                            notpackages.setVisibility(View.VISIBLE);
                            e.printStackTrace();
                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                        finally {
                            mySwipeRefreshLayout.setRefreshing(false);
                        }
                    }
                }
        );


        try {
            prepareServerPackages();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (JSONException e) {
            e.printStackTrace();
        }
        finally {

            mySwipeRefreshLayout.setRefreshing(false);
        }

        return inflated;
    }


    private void prepareServerPackages() throws FileNotFoundException, JSONException {
        String file_name = "/data/data/org.opm.openpackagemanager/app_bin/packages.list";

        FileInputStream fis = new FileInputStream(new File(file_name));
        InputStreamReader isr = new InputStreamReader(fis);

        BufferedReader bufferedReader = new BufferedReader(isr);
        StringBuilder sb = new StringBuilder();
        String line;
        try {
            while ((line = bufferedReader.readLine()) != null) {
                sb.append(line);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        Log.d("DIR",sb.toString());

        JSONArray packages = new JSONObject(sb.toString()).getJSONArray("fetched");
        packageList.clear();
        for (int i=0;i<packages.length();i++){
            JSONObject arr_package = packages.getJSONObject(i);
            Package a = new Package(arr_package.getString("packagename"), arr_package.getString("version"), false);
            packageList.add(a);

        }
        adapter.notifyDataSetChanged();
    }

    // TODO: Rename method, update argument and hook method into UI event
    public void onButtonPressed(Uri uri) {
        if (mListener != null) {
            mListener.onFragmentInteraction(uri);
        }
    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        if (context instanceof OnFragmentInteractionListener) {
            mListener = (OnFragmentInteractionListener) context;
        } else {
            throw new RuntimeException(context.toString()
                    + " must implement OnFragmentInteractionListener");
        }
    }

    @Override
    public void onDetach() {
        super.onDetach();
        mListener = null;
    }

    /**
     * This interface must be implemented by activities that contain this
     * fragment to allow an interaction in this fragment to be communicated
     * to the activity and potentially other fragments contained in that
     * activity.
     * <p>
     * See the Android Training lesson <a href=
     * "http://developer.android.com/training/basics/fragments/communicating.html"
     * >Communicating with Other Fragments</a> for more information.
     */
    public interface OnFragmentInteractionListener {
        // TODO: Update argument type and name
        void onFragmentInteraction(Uri uri);
    }
}
