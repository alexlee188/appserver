package com.alexlee188.sgnurse;

import java.io.IOException;
import java.io.StringReader;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Locale;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.support.v7.app.ActionBarActivity;
import android.support.v7.app.ActionBar;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v4.app.FragmentStatePagerAdapter;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v4.view.ViewPager;
import android.text.TextUtils;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.ListView;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;
import android.widget.Toast;
import android.content.Context;
import android.content.SharedPreferences;

import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.gcm.GoogleCloudMessaging;
import com.google.android.gms.common.ConnectionResult;
import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;
import org.xmlpull.v1.XmlPullParserFactory;



public class MainActivity extends ActionBarActivity implements
		ActionBar.TabListener {

	SectionsPagerAdapter mSectionsPagerAdapter;

	/**
	 * The {@link ViewPager} that will host the section contents.
	 */
	ViewPager mViewPager;
	
    static ArrayList <job> list_values = new ArrayList<job>();
    static ArrayList <job> assigned_values = new ArrayList<job>();

    private final static int PLAY_SERVICES_RESOLUTION_REQUEST = 9000;

    Boolean isPaused;
    GoogleCloudMessaging gcm;
    String regId;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

        list_values.add(new job("","", "", "Waiting for server update..."));
        assigned_values.add(new job("","", "", "Waiting for server update..."));

        regId = registerGCM();

		// Set up the action bar.
		final ActionBar actionBar = getSupportActionBar();
		actionBar.setNavigationMode(ActionBar.NAVIGATION_MODE_TABS);

		// Create the adapter that will return a fragment for each of the three
		// primary sections of the activity.
		mSectionsPagerAdapter = new SectionsPagerAdapter(
				getSupportFragmentManager());

		// Set up the ViewPager with the sections adapter.
		mViewPager = (ViewPager) findViewById(R.id.pager);
		mViewPager.setAdapter(mSectionsPagerAdapter);

		// When swiping between different sections, select the corresponding
		// tab. We can also use ActionBar.Tab#select() to do this if we have
		// a reference to the Tab.
		mViewPager
				.setOnPageChangeListener(new ViewPager.SimpleOnPageChangeListener() {
					@Override
					public void onPageSelected(int position) {
						actionBar.setSelectedNavigationItem(position);
					}
				});

		// For each of the sections in the app, add a tab to the action bar.
		for (int i = 0; i < mSectionsPagerAdapter.getCount(); i++) {
			// Create a tab with text corresponding to the page title defined by
			// the adapter. Also specify this Activity object, which implements
			// the TabListener interface, as the callback (listener) for when
			// this tab is selected.
			actionBar.addTab(actionBar.newTab()
					.setText(mSectionsPagerAdapter.getPageTitle(i))
					.setTabListener(this));
		}
        new GetAccountTask().execute("");
	}

    @Override
    public void onPause(){
        super.onPause();
        isPaused = true;
    }

    @Override
    public void onResume(){
        super.onResume();
        isPaused = false;
    }

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {

		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
        else if (id == R.id.action_about){
            AboutDialog aboutDialog = new AboutDialog(this);
            aboutDialog.setTitle("About SGnurse");
            aboutDialog.show();
            return true;
        }
		return super.onOptionsItemSelected(item);
	}

	@Override
	public void onTabSelected(ActionBar.Tab tab,
			FragmentTransaction fragmentTransaction) {
		// When the given tab is selected, switch to the corresponding page in
		// the ViewPager.
		mViewPager.setCurrentItem(tab.getPosition());
        // Get list of available jobs
        if (tab.getPosition()== 0){     // updates the job available list when tab selected
            new GetJobsTask().execute("");
        } else if (tab.getPosition()== 1){
            new GetAssignedJobsTask().execute("");
        } else if (tab.getPosition()== 3) {
            new GetAccountTask().execute("");
        }

	}

	@Override
	public void onTabUnselected(ActionBar.Tab tab,
			FragmentTransaction fragmentTransaction) {
	}

	@Override
	public void onTabReselected(ActionBar.Tab tab,
			FragmentTransaction fragmentTransaction) {
	}

    /**
     * Check the device to make sure it has the Google Play Services APK. If
     * it doesn't, display a dialog that allows users to download the APK from
     * the Google Play Store or enable it in the device's system settings.
     */
    private boolean checkPlayServices() {
        int resultCode = GooglePlayServicesUtil.isGooglePlayServicesAvailable(this);
        if (resultCode != ConnectionResult.SUCCESS) {
            if (GooglePlayServicesUtil.isUserRecoverableError(resultCode)) {
                GooglePlayServicesUtil.getErrorDialog(resultCode, this,
                        PLAY_SERVICES_RESOLUTION_REQUEST).show();
            } else {
;
                finish();
            }
            return false;
        }
        return true;
    }

	/**
	 * A {@link FragmentPagerAdapter} that returns a fragment corresponding to
	 * one of the sections/tabs/pages.
	 */
	public class SectionsPagerAdapter extends FragmentStatePagerAdapter {

		public SectionsPagerAdapter(FragmentManager fm) {
			super(fm);
		}

		@Override
		public Fragment getItem(int position) {
			// getItem is called to instantiate the fragment for the given page.
			// Return a PlaceholderFragment (defined as a static inner class
			// below).
			return PlaceholderFragment.newInstance(position + 1);
		}

		@Override
		public int getCount() {
			// Show 4 total pages.
			return 4;
		}

		@Override
		public CharSequence getPageTitle(int position) {
			Locale l = Locale.getDefault();
			switch (position) {
			case 0:
				return getString(R.string.title_section1).toUpperCase(l);
			case 1:
				return getString(R.string.title_section2).toUpperCase(l);
			case 2:
				return getString(R.string.title_section3).toUpperCase(l);
			case 3:
				return getString(R.string.title_section4).toUpperCase(l);
			}
			return null;
		}
		
		@Override
		public int getItemPosition(Object item) {
	        PlaceholderFragment fragment = (PlaceholderFragment)item;
	        if (fragment.getArguments().getInt(ARG_SECTION_NUMBER) == 1){
	            return POSITION_NONE;
	        } else if (fragment.getArguments().getInt(ARG_SECTION_NUMBER) == 2){
                return POSITION_NONE;
            } else if (fragment.getArguments().getInt(ARG_SECTION_NUMBER) == 4){
                return POSITION_NONE;
            }
	        else return POSITION_UNCHANGED;
	    }
		
		/**
		 * The fragment argument representing the section number for this
		 * fragment.
		 */
		private static final String ARG_SECTION_NUMBER = "section_number";
	}

	/**
	 * A placeholder fragment containing a simple view.
	 */
	public static class PlaceholderFragment extends Fragment {
		/**
		 * The fragment argument representing the section number for this
		 * fragment.
		 */
		private static final String ARG_SECTION_NUMBER = "section_number";
		
		/**
		 * Returns a new instance of this fragment for the given section number.
		 */
		public static PlaceholderFragment newInstance(int sectionNumber) {
			PlaceholderFragment fragment = new PlaceholderFragment();
			Bundle args = new Bundle();
			args.putInt(ARG_SECTION_NUMBER, sectionNumber);
			fragment.setArguments(args);
			return fragment;
		}

		public PlaceholderFragment() {
		}

		@Override
		public View onCreateView(LayoutInflater inflater, ViewGroup container,
				Bundle savedInstanceState) {
			if (getArguments().getInt(ARG_SECTION_NUMBER) == 1) {

                View rootView = inflater.inflate(R.layout.fragment_list, container,
                        false);

                final ListView listView = (ListView) rootView.findViewById(R.id.fragment_list);

                // Define a new Adapter
                // First parameter - Context
                // Second parameter - Layout for the row
                // Third parameter - ID of the TextView to which the data is written
                // Forth - the Array of data

                // ArrayAdapter needs to be associated with the activity.
                // If used in a fragment, the activity is not NULL only after the fragment is
                // attached
                JobAdapter adapter = new JobAdapter(getActivity().getBaseContext(), list_values);
                // Assign adapter to ListView
                listView.setAdapter(adapter);

                // ListView Item Click Listener
                listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {

                    @Override
                    public void onItemClick(AdapterView<?> parent, View view,
                                            int position, long id) {
                        // ListView Clicked item index
                        job itemValue = (job) listView.getItemAtPosition(position);

                        AlertDialog.Builder adb;
                        AlertDialog ad;

                        adb = new AlertDialog.Builder(getActivity());

                        adb.setTitle("Request Job Assignment");
                        final SharedPreferences prefs =
                                getActivity().getSharedPreferences(MainActivity.class.getSimpleName(),
                                        Context.MODE_PRIVATE);
                        SharedPreferences.Editor editor = prefs.edit();
                        editor.putString("JOB_ID", itemValue.get_job_id() );
                        editor.commit();
                        float bal = Float.parseFloat(prefs.getString("ACCOUNT_BALANCE", "0.00"));
                        if (bal < 2.0f) {
                            adb.setMessage("Your Account Balance is less than $2.00.  " +
                            "Please update your info in 'MY DETAILS' tab.  " +
                            "Also make sure you have verified your qualifications and credentials " +
                            "by calling SGnurse admin at 97851440.");
                            adb.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int which) {
                                    dialog.dismiss();
                                }
                            });
                        } else {
                            adb.setMessage("Update your info in 'MY DETAILS' tab.  " +
                             "Also make sure you have verified your qualifications and credentials " +
                             "by calling SGnurse admin at 97851440.  " +
                             "Do you wish to proceed with request to assign job: " +
                             itemValue.get_job_date_time() + " " +
                             itemValue.get_job_details() + " ?"
                            );
                            adb.setNegativeButton("CANCEL", new DialogInterface.OnClickListener(){
                                public void onClick(DialogInterface dialog, int which) {
                                    dialog.dismiss();
                                }
                            });
                            adb.setPositiveButton("PROCEED", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int which) {
                                    AsyncTask <String, String, Void> AssignJobTask = new AsyncTask<String,String,Void>() {
                                        private AlertDialog.Builder adb;
                                        private AlertDialog ad;

                                        @Override
                                        protected void onPreExecute(){
                                            super.onPreExecute();
                                            adb = new AlertDialog.Builder(getActivity());
                                            adb.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                                public void onClick(DialogInterface dialog, int which) {
                                                    dialog.dismiss();
                                                }
                                            });
                                            adb.setTitle("Assign Job Request");
                                        }

                                        @Override
                                        protected void onPostExecute(Void result){
                                            super.onPostExecute(result);
                                            ad = adb.create();
                                            ad.show();
                                        }

                                        @Override
                                        protected Void doInBackground(String... message) {
                                            final SharedPreferences prefs =
                                                    getActivity().getSharedPreferences(MainActivity.class.getSimpleName(),
                                                            Context.MODE_PRIVATE);
                                            String regId = prefs.getString("REG_ID", "");
                                            String job_id = prefs.getString("JOB_ID", "");
                                            String xml_msg =  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" +
                                                    "<ASSIGN gcm_regid=\"" + regId +
                                                    "\" job_id=\"" + job_id + "\"></ASSIGN>";
                                            String xml = String.format("%04d",xml_msg.length()+4) + xml_msg;
                                            //we create a TCPClient object and
                                            TCPClient mTcpClient = new TCPClient(new TCPClient.OnMessageReceived() {
                                                @Override
                                                //here the messageReceived method is implemented
                                                public void messageReceived(String message) {
                                                    //this method calls the onProgressUpdate
                                                    publishProgress(message);
                                                }
                                            }, xml);
                                            mTcpClient.run();
                                            return null;
                                        }

                                        @Override
                                        protected void onProgressUpdate(String... values) {
                                            super.onProgressUpdate(values);
                                            XmlPullParserFactory factory;
                                            try {
                                                factory = XmlPullParserFactory.newInstance();
                                                factory.setNamespaceAware(true);
                                                XmlPullParser xpp = factory.newPullParser();
                                                xpp.setInput(new StringReader(values[0]));
                                                int eventType = xpp.getEventType();
                                                while (eventType != XmlPullParser.END_DOCUMENT) {
                                                    if(eventType == XmlPullParser.START_DOCUMENT) {
                                                    } else if(eventType == XmlPullParser.START_TAG) {
                                                        if (xpp.getName().equalsIgnoreCase("ASSIGN")) {
                                                            eventType = xpp.next();
                                                            if (eventType == XmlPullParser.TEXT) {
                                                                if (xpp.getText().equalsIgnoreCase("fail")) {
                                                                    adb.setMessage("FAIL - Request to assign job failed. " +
                                                                            "This job may have already been assigned to another user. " +
                                                                            "Update your info in 'MY DETAILS' tab.  Also make sure you have " +
                                                                            "verified your qualifications and credentials.  Call SGnurse admin at" +
                                                                            " 97851440.");
                                                                } else if (xpp.getText().equalsIgnoreCase("success")) {
                                                                    adb.setMessage("SUCCESS - SGnurse admin will call you to confirm. " +
                                                                            "Make sure your info in 'MY DETAILS' is correct.");
                                                                }
                                                            }
                                                        }
                                                    } else if(eventType == XmlPullParser.END_TAG) {
                                                        if (xpp.getName().equalsIgnoreCase("ASSIGN")){
                                                        }
                                                    }
                                                    eventType = xpp.next();
                                                }
                                            } catch (XmlPullParserException e) {
                                                // TODO Auto-generated catch block
                                                e.printStackTrace();
                                            } catch (IOException e) {
                                                // TODO Auto-generated catch block
                                                e.printStackTrace();
                                            }
                                        }
                                    };	// end AssignJobTask
                                    AssignJobTask.execute();
                                    dialog.dismiss();
                                }
                            });
                        }
                        ad = adb.create();
                        ad.show();
                    }
                });
                return rootView;
            } else if (getArguments().getInt(ARG_SECTION_NUMBER) == 2){

                    View rootView = inflater.inflate(R.layout.fragment_assigned, container,
                            false);

                    final ListView listView = (ListView) rootView.findViewById(R.id.assigned_list);

                // ArrayAdapter needs to be associated with the activity.
                // If used in a fragment, the activity is not NULL only after the fragment is
                // attached
                AssignedJobAdapter adapter = new AssignedJobAdapter(getActivity().getBaseContext(), assigned_values);
                    // Assign adapter to ListView
                    listView.setAdapter(adapter);

                    // ListView Item Click Listener
                    listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {

                        @Override
                        public void onItemClick(AdapterView<?> parent, View view,
                                                int position, long id) {

                            job itemValue = (job)listView.getItemAtPosition(position);

                            AlertDialog.Builder adb;
                            AlertDialog ad;

                            adb = new AlertDialog.Builder(getActivity());

                            adb.setTitle("Job Assignment Details");

                            StringBuilder sb = new StringBuilder();
                            sb.append("Customer Name: " + itemValue.getCustomer_name_1() + "/ " +
                                    itemValue.getCustomer_name_2());
                            sb.append("\n");
                            sb.append("Address: ");
                            sb.append(itemValue.getCustomer_addr_blk_no());
                            sb.append(" ");
                            sb.append(itemValue.getCustomer_addr_street_1());
                            sb.append("\n        ");
                            sb.append(itemValue.getCustomer_addr_street_2());
                            sb.append("\nPhone: ");
                            sb.append(itemValue.getCustomer_phone());
                            sb.append("\nMobile: ");
                            sb.append(itemValue.getCustomer_mobile());
                            sb.append("\nJob Date/time: ");
                            sb.append(itemValue.get_job_date_time());
                            sb.append("\nJob Details: ");
                            sb.append(itemValue.get_job_details());
                            adb.setMessage(sb.toString());
                            adb.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int which) {
                                    dialog.dismiss();
                                }
                            });
                            // Show Alert
                            ad = adb.create();
                            ad.show();

                        }

                    });
                    return rootView;
            } else if (getArguments().getInt(ARG_SECTION_NUMBER) == 3){
                final SharedPreferences prefs =
                        getActivity().getSharedPreferences(MainActivity.class.getSimpleName(),
                                Context.MODE_PRIVATE);
                View rootView = inflater.inflate(R.layout.fragment_user_details, container,
                        false);
                final TextView name = (TextView) rootView.findViewById(R.id.userName);
                final TextView email = (TextView) rootView.findViewById(R.id.userEmail);
                final TextView phone = (TextView) rootView.findViewById(R.id.userPhone);
                final TextView NRIC = (TextView) rootView.findViewById(R.id.NRIC);
                //final TextView date_of_birth = (TextView) rootView.findViewById(R.id.date_of_birth);
                final RadioGroup gender = (RadioGroup) rootView.findViewById(R.id.gender_group);
                final RadioButton male = (RadioButton) rootView.findViewById(R.id.male_btn);
                final RadioButton female = (RadioButton) rootView.findViewById(R.id.female_btn);
                final RadioGroup nurse_type = (RadioGroup) rootView.findViewById(R.id.nurse_type_group);
                final RadioButton RN = (RadioButton) rootView.findViewById(R.id.RN_btn);
                final RadioButton EN = (RadioButton) rootView.findViewById(R.id.EN_btn);
                final RadioButton NN = (RadioButton) rootView.findViewById(R.id.NN_btn);
                final CheckBox have_insurance = (CheckBox) rootView.findViewById(R.id.insurance_checkBox);

                name.setText(prefs.getString("USER_NAME", ""));
                email.setText(prefs.getString("USER_EMAIL", ""));
                phone.setText(prefs.getString("USER_PHONE", ""));
                NRIC.setText(prefs.getString("USER_NRIC", ""));
                //date_of_birth.setText(prefs.getString("USER_BIRTH", ""));
                if (prefs.getInt("USER_GENDER", 1) == 1) gender.check(female.getId());
                else gender.check(male.getId());
                if (prefs.getInt("USER_TYPE", 2 ) == 2) {
                    nurse_type.check(NN.getId());
                }
                else if (prefs.getInt("USER_TYPE", 2) == 1) {
                    nurse_type.check(EN.getId());
                }
                else {
                    nurse_type.check(RN.getId());
                }
                have_insurance.setChecked(prefs.getBoolean("USER_INSURANCE", false));

                Button button = (Button) rootView.findViewById(R.id.userUpdate);
                button.setOnClickListener(new View.OnClickListener() {
                    public void onClick(View v) {
                        SharedPreferences prefs =
                                getActivity().getSharedPreferences(MainActivity.class.getSimpleName(),
                                        Context.MODE_PRIVATE);
                        SharedPreferences.Editor editor = prefs.edit();
                        //editor.putString(REG_ID, regId);
                        //editor.putInt(APP_VERSION, appVersion);
                        editor.putString("USER_NAME", name.getText().toString());
                        editor.putString("USER_EMAIL", email.getText().toString());
                        editor.putString("USER_PHONE", phone.getText().toString());
                        editor.putString("USER_NRIC", NRIC.getText().toString());
                        editor.putString("USER_BIRTH", "2000-01-01");
                        editor.putInt("USER_GENDER", male.isChecked() ? 0:1);
                        if (NN.isChecked()) editor.putInt("USER_TYPE", 2);
                        else if (EN.isChecked()) editor.putInt("USER_TYPE", 1);
                        else editor.putInt("USER_TYPE", 0);
                        editor.putBoolean("USER_INSURANCE", have_insurance.isChecked());
                        editor.commit();

                        AsyncTask <String, String, Void> InsertRegIdTask = new AsyncTask<String,String,Void>() {
                            @Override
                            protected Void doInBackground(String... message) {
                                final SharedPreferences prefs =
                                        getActivity().getSharedPreferences(MainActivity.class.getSimpleName(),
                                                Context.MODE_PRIVATE);
                                String regId = prefs.getString("REG_ID", "");
                                String user_name = prefs.getString("USER_NAME", "noname");
                                String user_email = prefs.getString("USER_EMAIL", "who@somewhere.com");
                                String user_phone = prefs.getString("USER_PHONE", "");
                                String user_NRIC = prefs.getString("USER_NRIC", "");
                                String user_birth = prefs.getString("USER_BIRTH", "");
                                String user_gender = prefs.getInt("USER_GENDER", 1) == 1 ? "female": "male";
                                String user_type = "NN";
                                if ( prefs.getInt("USER_TYPE", 2)  == 0) user_type = "RN";
                                else if (prefs.getInt("USER_TYPE", 2) == 1) user_type = "EN";
                                String user_insurance = prefs.getBoolean("USER_INSURANCE", false)?
                                        "1": "0";

                                String xml_msg =  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" +
                                        "<INSERT gcm_regid=\"" + regId + "\" name=\"" + user_name +
                                        "\" email=\"" + user_email + "\" phone=\"" +
                                        user_phone + "\" NRIC=\"" + user_NRIC + "\" date_of_birth=\"" +
                                        user_birth + "\" gender=\"" + user_gender + "\" nurse_type=\""  +
                                        user_type + "\" have_insurance=\"" + user_insurance +
                                        "\"></INSERT>";
                                String xml = String.format("%04d",xml_msg.length()+4) + xml_msg;
                                //we create a TCPClient object and
                                TCPClient mTcpClient = new TCPClient(new TCPClient.OnMessageReceived() {
                                    @Override
                                    //here the messageReceived method is implemented
                                    public void messageReceived(String message) {
                                        //this method calls the onProgressUpdate
                                        publishProgress(message);
                                    }
                                }, xml);
                                mTcpClient.run();
                                return null;
                            }

                            @Override
                            protected void onProgressUpdate(String... values) {
                                super.onProgressUpdate(values);

                                ArrayList<String> list = new ArrayList<String>();
                                XmlPullParserFactory factory;
                                try {
                                    factory = XmlPullParserFactory.newInstance();
                                    factory.setNamespaceAware(true);
                                    XmlPullParser xpp = factory.newPullParser();
                                    xpp.setInput(new StringReader(values[0]));
                                    int eventType = xpp.getEventType();
                                    while (eventType != XmlPullParser.END_DOCUMENT) {
                                        if(eventType == XmlPullParser.START_DOCUMENT) {
                                        } else if(eventType == XmlPullParser.START_TAG) {
                                            if (xpp.getName().equalsIgnoreCase("INSERT")){
                                            }
                                        } else if(eventType == XmlPullParser.TEXT) {
                                        } else if(eventType == XmlPullParser.END_TAG) {
                                            if (xpp.getName().equalsIgnoreCase("INSERT")){
                                            }
                                        }
                                        eventType = xpp.next();
                                    }
                                } catch (XmlPullParserException e) {
                                    // TODO Auto-generated catch block
                                    e.printStackTrace();
                                } catch (IOException e) {
                                    // TODO Auto-generated catch block
                                    e.printStackTrace();
                                }
                            }
                        };	// end InsertRegIdTask

                        String regId = prefs.getString("REG_ID", "");
                        if ( !TextUtils.isEmpty(regId)){
                            InsertRegIdTask.execute();
                        }
                    } // end onClick
                });
                return rootView;
            } else if (getArguments().getInt(ARG_SECTION_NUMBER) == 4){
                View rootView = inflater.inflate(R.layout.fragment_account, container,
                        false);
                final TextView balance = (TextView) rootView.findViewById(R.id.accountBalance);
                final SharedPreferences prefs =
                        getActivity().getSharedPreferences(MainActivity.class.getSimpleName(),
                                Context.MODE_PRIVATE);
                String bal = prefs.getString("ACCOUNT_BALANCE", "0.00");
                balance.setText("$" + bal);
                return rootView;
			} else {
			View rootView = inflater.inflate(R.layout.fragment_main, container,
					false);
			TextView textView = (TextView) rootView
					.findViewById(R.id.section_label);
			textView.setText(Integer.toString(getArguments().getInt(
					ARG_SECTION_NUMBER)));
			return rootView;
			}
		}
	}
	
	public class GetJobsTask extends AsyncTask<String,String,Void> {
	    	 
        @Override
        protected Void doInBackground(String... message) {
            //we create a TCPClient object and
            TCPClient mTcpClient = new TCPClient(new TCPClient.OnMessageReceived() {
                @Override
                //here the messageReceived method is implemented
                public void messageReceived(String message) {
                    //this method calls the onProgressUpdate
                    publishProgress(message);
                }
            }, "0065<?xml version=\"1.0\" encoding=\"UTF-8\"?> "
                    + "<QUERY>GetJobs</QUERY>");
            mTcpClient.run();
            return null;
        }
 
        @Override
        protected void onProgressUpdate(String... values) {
            super.onProgressUpdate(values);

            list_values.clear();

            XmlPullParserFactory factory;
            StringBuilder s = null;
            String job_id = "";
            String post_district = "";
            String job_date_time = "";
			try {
				factory = XmlPullParserFactory.newInstance();
				factory.setNamespaceAware(true);
				XmlPullParser xpp = factory.newPullParser();
				xpp.setInput(new StringReader(values[0]));
		        int eventType = xpp.getEventType();
		        while (eventType != XmlPullParser.END_DOCUMENT) {
		          if(eventType == XmlPullParser.START_DOCUMENT) {
		          } else if(eventType == XmlPullParser.START_TAG) {
		        	  if (xpp.getName().equalsIgnoreCase("JOB")){
		        		  s = new StringBuilder();
		        	  }
		        	  else if (xpp.getName().equalsIgnoreCase("ADDR_POSTCODE")){
		        		  eventType = xpp.next();
		        		  if(eventType == XmlPullParser.TEXT){
		        			  String dist = xpp.getText().substring(0, 2);
                              String post_code = xpp.getText();
                              post_district = post_district_from_postcode(dist, post_code);
		        		  }
		        	  }
                      else if (xpp.getName().equalsIgnoreCase("JOB_ID")){
                          eventType = xpp.next();
                          if(eventType == XmlPullParser.TEXT){
                               job_id = xpp.getText();
                          }
                      }
		        	  else if (xpp.getName().equalsIgnoreCase("JOB_DESC")){
		        		  eventType = xpp.next();
		        		  if(eventType == XmlPullParser.TEXT) s.append(xpp.getText());
		        	  }
		        	  else if (xpp.getName().equalsIgnoreCase("NEED")){
		        		  eventType = xpp.next();
		        		  if(eventType == XmlPullParser.TEXT) {
                              s.append(" Needs:");
                              s.append(xpp.getText());
                          };
		        	  }	
		        	  else if (xpp.getName().equalsIgnoreCase("JOB_START_TIME")){
		        		  eventType = xpp.next();
		        		  if(eventType == XmlPullParser.TEXT) {
                              job_date_time = xpp.getText().substring(0, 16);
                          }
		        	  }
		        	  else if (xpp.getName().equalsIgnoreCase("JOB_DURATION")){
		        		  eventType = xpp.next();
		        		  if(eventType == XmlPullParser.TEXT) {
                              s.append(" for ");
                              s.append(xpp.getText());
                          }
		        	  }	
		          } else if(eventType == XmlPullParser.TEXT) {
		          } else if(eventType == XmlPullParser.END_TAG) {
		        	  if (xpp.getName().equalsIgnoreCase("JOB")){
		        		  list_values.add(new job(job_id, job_date_time, post_district, s.toString()));
                          job_id = "";
                          job_date_time = "";
                          post_district = "";
		        		  s = new StringBuilder();
		        	  }
		          }
		          eventType = xpp.next();
		         }
			} catch (XmlPullParserException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

            if (!isPaused){ // Async task is still running when main activity may be paused
                            // calling the GUI methods will segfault
                mSectionsPagerAdapter.notifyDataSetChanged();
            }
        }
	}	// end GetJobTask

    public class GetAssignedJobsTask extends AsyncTask<String,String,Void> {

        @Override
        protected Void doInBackground(String... message) {
            //we create a TCPClient object and
            TCPClient mTcpClient = new TCPClient(new TCPClient.OnMessageReceived() {
                @Override
                //here the messageReceived method is implemented
                public void messageReceived(String message) {
                    //this method calls the onProgressUpdate
                    publishProgress(message);
                }
            }, "0240<?xml version=\"1.0\" encoding=\"UTF-8\"?> "
                    + "<QUERY gcm_regid=\"" + regId + "\">GetJobs</QUERY>");
            mTcpClient.run();
            return null;
        }

        @Override
        protected void onProgressUpdate(String... values) {
            super.onProgressUpdate(values);

            assigned_values.clear();

            XmlPullParserFactory factory;
            StringBuilder s = null;
            String job_id = "";
            String job_status = "";
            String post_district = "";
            String job_date_time = "";
            String customer_name_1 = "";
            String customer_name_2 = "";
            String customer_addr_blk_no = "";
            String customer_addr_street_1 = "";
            String customer_addr_street_2 = "";
            String customer_phone = "";
            String customer_mobile = "";
            try {
                factory = XmlPullParserFactory.newInstance();
                factory.setNamespaceAware(true);
                XmlPullParser xpp = factory.newPullParser();
                xpp.setInput(new StringReader(values[0]));
                int eventType = xpp.getEventType();
                while (eventType != XmlPullParser.END_DOCUMENT) {
                    if(eventType == XmlPullParser.START_DOCUMENT) {
                    } else if(eventType == XmlPullParser.START_TAG) {
                        if (xpp.getName().equalsIgnoreCase("JOB")){
                            s = new StringBuilder();
                            job_status = xpp.getAttributeValue("", "STATUS");
                        }
                        else if (xpp.getName().equalsIgnoreCase("JOB_ID")){
                            eventType = xpp.next();
                            if(eventType == XmlPullParser.TEXT){
                                job_id = xpp.getText();
                            }
                        }
                        else if (xpp.getName().equalsIgnoreCase("ADDR_POSTCODE")){
                            eventType = xpp.next();
                            if(eventType == XmlPullParser.TEXT){
                                String dist = xpp.getText().substring(0, 2);
                                String post_code = xpp.getText();
                                post_district = post_district_from_postcode(dist, post_code);
                            }
                        }
                        else if (xpp.getName().equalsIgnoreCase("JOB_DESC")){
                            eventType = xpp.next();
                            if(eventType == XmlPullParser.TEXT) s.append(xpp.getText());
                        }
                        else if (xpp.getName().equalsIgnoreCase("NEED")){
                            eventType = xpp.next();
                            if(eventType == XmlPullParser.TEXT) {
                                s.append(" Needs:");
                                s.append(xpp.getText());
                            };
                        }
                        else if (xpp.getName().equalsIgnoreCase("JOB_START_TIME")){
                            eventType = xpp.next();
                            if(eventType == XmlPullParser.TEXT) {
                                job_date_time = xpp.getText().substring(0,16);
                            }
                        }
                        else if (xpp.getName().equalsIgnoreCase("JOB_DURATION")){
                            eventType = xpp.next();
                            if(eventType == XmlPullParser.TEXT) {
                                s.append(" for ");
                                s.append(xpp.getText());
                            }
                        }
                        else if (xpp.getName().equalsIgnoreCase("NAME_1")){
                            eventType = xpp.next();
                            if(eventType == XmlPullParser.TEXT) {
                                customer_name_1= xpp.getText();
                            }
                        }
                        else if (xpp.getName().equalsIgnoreCase("NAME_2")){
                            eventType = xpp.next();
                            if(eventType == XmlPullParser.TEXT) {
                                customer_name_2= xpp.getText();
                                if (customer_name_2.equalsIgnoreCase("NULL")) customer_name_2 = "";
                            }
                        }
                        else if (xpp.getName().equalsIgnoreCase("ADDR_BLK_NO")){
                            eventType = xpp.next();
                            if(eventType == XmlPullParser.TEXT) {
                                customer_addr_blk_no= xpp.getText();
                                if (customer_addr_blk_no.equalsIgnoreCase("NULL")) customer_addr_blk_no = "";
                            }
                        }
                        else if (xpp.getName().equalsIgnoreCase("ADDR_STREET_1")){
                            eventType = xpp.next();
                            if(eventType == XmlPullParser.TEXT) {
                                customer_addr_street_1= xpp.getText();
                                if (customer_addr_street_1.equalsIgnoreCase("NULL"))
                                    customer_addr_street_1 = "";
                            }
                        }
                        else if (xpp.getName().equalsIgnoreCase("ADDR_STREET_2")){
                            eventType = xpp.next();
                            if(eventType == XmlPullParser.TEXT) {
                                customer_addr_street_2= xpp.getText();
                                if (customer_addr_street_2.equalsIgnoreCase("NULL"))
                                    customer_addr_street_2 = "";
                            }
                        }
                        else if (xpp.getName().equalsIgnoreCase("PHONE")){
                            eventType = xpp.next();
                            if(eventType == XmlPullParser.TEXT) {
                                customer_phone= xpp.getText();
                                if (customer_phone.equalsIgnoreCase("NULL"))
                                    customer_phone = "";
                            }
                        }
                        else if (xpp.getName().equalsIgnoreCase("MOBILE")){
                            eventType = xpp.next();
                            if(eventType == XmlPullParser.TEXT) {
                                customer_mobile= xpp.getText();
                                if (customer_mobile.equalsIgnoreCase("NULL"))
                                    customer_mobile = "";
                            }
                        }
                    } else if(eventType == XmlPullParser.TEXT) {
                    } else if(eventType == XmlPullParser.END_TAG) {
                        if (xpp.getName().equalsIgnoreCase("JOB")){
                            assigned_values.add(new job(job_id, job_status, job_date_time, post_district, s.toString(),
                                    customer_name_1, customer_name_2, customer_addr_blk_no,
                                    customer_addr_street_1, customer_addr_street_2, customer_phone,
                                    customer_mobile));
                            job_id = "";
                            job_status = "";
                            job_date_time = "";
                            post_district = "";
                            customer_name_1 = "";
                            customer_name_2 = "";
                            customer_addr_blk_no = "";
                            customer_addr_street_1 = "";
                            customer_addr_street_2 = "";
                            customer_phone = "";
                            customer_mobile = "";
                            s = new StringBuilder();
                        }
                    }
                    eventType = xpp.next();
                }
            } catch (XmlPullParserException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }

            if (!isPaused){ // Async task is still running when main activity may be paused
                // calling the GUI methods will segfault
                mSectionsPagerAdapter.notifyDataSetChanged();
            }
        }

    }

    public class GetAccountTask extends AsyncTask<String,String,Void> {
        @Override
        protected Void doInBackground(String... message) {
            final SharedPreferences prefs = getSharedPreferences(
                    MainActivity.class.getSimpleName(), Context.MODE_PRIVATE);
            String regId = prefs.getString("REG_ID", "");
            String xml_msg =  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" +
                    "<QUERY gcm_regid=\"" + regId + "\">GetAccount</QUERY>";
            String xml = String.format("%04d",xml_msg.length()+4) + xml_msg;
            //we create a TCPClient object and
            TCPClient mTcpClient = new TCPClient(new TCPClient.OnMessageReceived() {
                @Override
                //here the messageReceived method is implemented
                public void messageReceived(String message) {
                    //this method calls the onProgressUpdate
                    publishProgress(message);
                }
            }, xml);
            mTcpClient.run();
            return null;
        }

        @Override
        protected void onProgressUpdate(String... values) {
            super.onProgressUpdate(values);
            final SharedPreferences prefs = getSharedPreferences(
                    MainActivity.class.getSimpleName(), Context.MODE_PRIVATE);
            SharedPreferences.Editor editor = prefs.edit();

            XmlPullParserFactory factory;
            try {
                factory = XmlPullParserFactory.newInstance();
                factory.setNamespaceAware(true);
                XmlPullParser xpp = factory.newPullParser();
                xpp.setInput(new StringReader(values[0]));
                int eventType = xpp.getEventType();
                while (eventType != XmlPullParser.END_DOCUMENT) {
                    if(eventType == XmlPullParser.START_DOCUMENT) {
                    } else if(eventType == XmlPullParser.START_TAG) {
                        if (xpp.getName().equalsIgnoreCase("ACCOUNT")){
                            editor.putString("ACCOUNT_BALANCE", xpp.getAttributeValue(null,"BALANCE").toString());
                            editor.commit();
                        }
                    } else if(eventType == XmlPullParser.TEXT) {
                    } else if(eventType == XmlPullParser.END_TAG) {
                        if (xpp.getName().equalsIgnoreCase("ACCOUNT")){
                        }
                    }
                    eventType = xpp.next();
                }
            } catch (XmlPullParserException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
            if (!isPaused){ // Async task is still running when main activity may be paused
                // calling the GUI methods will segfault
                mSectionsPagerAdapter.notifyDataSetChanged();
            }
        }
    };	// end GetAccountTask

    public String post_district_from_postcode(String dist, String post_code){
        String post_district = "";
        if (dist.equalsIgnoreCase("01")||
                dist.equalsIgnoreCase("02")||
                dist.equalsIgnoreCase("03")||
                dist.equalsIgnoreCase("04")||
                dist.equalsIgnoreCase("05")||
                dist.equalsIgnoreCase("06")||
                dist.equalsIgnoreCase("07")||
                dist.equalsIgnoreCase("08")){
            post_district = "Central";
        }
        else if (dist.equalsIgnoreCase("11")||
                dist.equalsIgnoreCase("12")||
                dist.equalsIgnoreCase("13")){
            post_district = "Pasir Panjang, Clementi";
        }
        else if (dist.equalsIgnoreCase("09")||
                dist.equalsIgnoreCase("10")){
            post_district = "Harbour Front";
        }
        else if (dist.equalsIgnoreCase("14")||
                dist.equalsIgnoreCase("15")||
                dist.equalsIgnoreCase("16")){
            post_district = "Queenstown, Tiong Bahru";
        }
        else if (dist.equalsIgnoreCase("17")){
            post_district = "High St, Beach Road";
        }
        else if (dist.equalsIgnoreCase("18")||
                dist.equalsIgnoreCase("19")){
            post_district = "Middle Rd, Golden Mile";
        }
        else if (dist.equalsIgnoreCase("20")||
                dist.equalsIgnoreCase("21")){
            post_district = "Little India";
        }
        else if (dist.equalsIgnoreCase("22")||
                dist.equalsIgnoreCase("23")){
            post_district = "Orchard, River Valley";
        }
        else if (dist.equalsIgnoreCase("24")||
                dist.equalsIgnoreCase("25")||
                dist.equalsIgnoreCase("26")||
                dist.equalsIgnoreCase("27")){
            post_district = "Ardmore, Bukit Timah, Holland Road, Tanglin";
        }
        else if (dist.equalsIgnoreCase("28")||
                dist.equalsIgnoreCase("29")||
                dist.equalsIgnoreCase("30")){
            post_district = "Watten Estate, Novena, Thomson";
        }
        else if (dist.equalsIgnoreCase("31")||
                dist.equalsIgnoreCase("32")||
                dist.equalsIgnoreCase("33")){
            post_district = "Balestier, Toa Payoh, Serangoon";
        }
        else if (dist.equalsIgnoreCase("34")||
                dist.equalsIgnoreCase("35")||
                dist.equalsIgnoreCase("36")||
                dist.equalsIgnoreCase("37")){
            post_district = "Macpherson, Braddell";
        }
        else if (dist.equalsIgnoreCase("38")||
                dist.equalsIgnoreCase("39")||
                dist.equalsIgnoreCase("40")||
                dist.equalsIgnoreCase("41")){
            post_district = "Geylang, Eunos";
        }
        else if (dist.equalsIgnoreCase("42")||
                dist.equalsIgnoreCase("43")||
                dist.equalsIgnoreCase("44")||
                dist.equalsIgnoreCase("45")){
            post_district = "Katong, Joo Chiat";
        }
        else if (dist.equalsIgnoreCase("49")||
                dist.equalsIgnoreCase("50")||
                dist.equalsIgnoreCase("81")){
            post_district = "Loyang, Changi";
        }
        else if (dist.equalsIgnoreCase("46")||
                dist.equalsIgnoreCase("47")||
                dist.equalsIgnoreCase("48")){
            post_district = "Bedok, Upper East Coast";
        }
        else if (dist.equalsIgnoreCase("51")||
                dist.equalsIgnoreCase("52")){
            post_district = "Tampines, Pasir Ris";
        }
        else if (dist.equalsIgnoreCase("53")||
                dist.equalsIgnoreCase("54")||
                dist.equalsIgnoreCase("55")||
                dist.equalsIgnoreCase("82")){
            post_district = "Serangoon Garden, Hougang, Ponggol";
        }
        else if (dist.equalsIgnoreCase("56")||
                dist.equalsIgnoreCase("57")){
            post_district = "Bishan, Ang Mo Kio";
        }
        else if (dist.equalsIgnoreCase("58")||
                dist.equalsIgnoreCase("59")){
            post_district = "Upper Bukit Timah, Clementi Park, Ulu Pandan";
        }
        else if (dist.equalsIgnoreCase("60")||
                dist.equalsIgnoreCase("61")||
                dist.equalsIgnoreCase("62")||
                dist.equalsIgnoreCase("63")||
                dist.equalsIgnoreCase("64")){
            post_district = "Jurong";
        }
        else if (dist.equalsIgnoreCase("65")||
                dist.equalsIgnoreCase("66")||
                dist.equalsIgnoreCase("67")||
                dist.equalsIgnoreCase("68")){
            post_district = "Hillview, Dairy Farm, Bukit Panjang, Choa Chu Kang";
        }
        else if (dist.equalsIgnoreCase("69")||
                dist.equalsIgnoreCase("70")||
                dist.equalsIgnoreCase("71")){
            post_district = "[Lim Chu Kang, Tengah";
        }
        else if (dist.equalsIgnoreCase("72")||
                dist.equalsIgnoreCase("73")){
            post_district = "Kranji, Woodgrove, Woodlands";
        }
        else if (dist.equalsIgnoreCase("77")||
                dist.equalsIgnoreCase("78")){
            post_district = "Upper Thomson, Springleaf";
        }
        else if (dist.equalsIgnoreCase("75")||
                dist.equalsIgnoreCase("76")){
            post_district = "Yishun, Sembawang";
        }
        else if (dist.equalsIgnoreCase("79")||
                dist.equalsIgnoreCase("80")){
            post_district = "Selectar";
        }
        else {
            post_district = "S"+post_code;
        }
        return post_district;
    }

    public String registerGCM() {
        gcm = GoogleCloudMessaging.getInstance(this);
        regId = getRegistrationId(getApplicationContext());

        if (TextUtils.isEmpty(regId)) {
            registerInBackground();
        };

        return regId;
    }

    private String getRegistrationId(Context context) {
        final SharedPreferences prefs = getSharedPreferences(
                MainActivity.class.getSimpleName(), Context.MODE_PRIVATE);
        String registrationId = prefs.getString("REG_ID", "");
        if (registrationId.isEmpty()) {
            return "";
        }
        int registeredVersion = prefs.getInt("APP_VERSION", Integer.MIN_VALUE);
        int currentVersion = getAppVersion(context);
        if (registeredVersion != currentVersion) {
            return "";
        }
        return registrationId;
    }

    private static int getAppVersion(Context context) {
        try {
            PackageInfo packageInfo = context.getPackageManager()
                    .getPackageInfo(context.getPackageName(), 0);
            return packageInfo.versionCode;
        } catch (PackageManager.NameNotFoundException e) {
            Log.d("RegisterActivity",
                    "I never expected this! Going down, going down!" + e);
            throw new RuntimeException(e);
        }
    }

    private void registerInBackground() {
        new AsyncTask<Void, Void, String>() {
            @Override
            protected String doInBackground(Void... params) {
                String msg = "";
                try {
                    if (gcm == null) {
                        gcm = GoogleCloudMessaging.getInstance(getApplicationContext());
                    }
                    regId = gcm.register(Config.GOOGLE_PROJECT_ID);
                    Log.d("RegisterActivity", "registerInBackground - regId: "
                            + regId);
                    msg = "Device registered, registration ID=" + regId;

                    storeRegistrationId(getApplicationContext(), regId);
                } catch (IOException ex) {
                    msg = "Error :" + ex.getMessage();
                    Log.d("RegisterActivity", "Error: " + msg);
                }
                Log.d("RegisterActivity", "AsyncTask completed: " + msg);
                return msg;
            }

            @Override
            protected void onPostExecute(String msg) {
                Toast.makeText(getApplicationContext(),
                        "Registered with GCM Server." + msg, Toast.LENGTH_LONG)
                        .show();
            }
        }.execute(null, null, null);
    }

    private void storeRegistrationId(Context context, String regId) {
        final SharedPreferences prefs = getSharedPreferences(
                MainActivity.class.getSimpleName(), Context.MODE_PRIVATE);
        int appVersion = getAppVersion(context);
        SharedPreferences.Editor editor = prefs.edit();
        editor.putString("REG_ID", regId);
        editor.putInt("APP_VERSION", appVersion);
        editor.commit();
    }

}

