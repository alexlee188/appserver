package com.alexlee188.sgnurse;

import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.Locale;

import android.support.v7.app.ActionBarActivity;
import android.support.v7.app.ActionBar;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v4.app.FragmentStatePagerAdapter;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v4.view.ViewPager;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;
import org.xmlpull.v1.XmlPullParserFactory;

public class MainActivity extends ActionBarActivity implements
		ActionBar.TabListener {

	/**
	 * The {@link android.support.v4.view.PagerAdapter} that will provide
	 * fragments for each of the sections. We use a {@link FragmentPagerAdapter}
	 * derivative, which will keep every loaded fragment in memory. If this
	 * becomes too memory intensive, it may be best to switch to a
	 * {@link android.support.v4.app.FragmentStatePagerAdapter}.
	 */
	SectionsPagerAdapter mSectionsPagerAdapter;

	/**
	 * The {@link ViewPager} that will host the section contents.
	 */
	ViewPager mViewPager;
	
	// The TCPClient for connecting to the server
	static TCPClient mTcpClient = null;
	
    static String[] list_values = new String[] { "Postcode:123123 Change of head dressing Needs: Wound Care Date-time:2014-10-11 10:30 for 2.0 HR",
    	"Postcode:909654 General Care Date-time:2014-11-30 18:00 for 4 HR"
    };
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		
		// Start the TCPClient to connect to server
		new ConnectTask().execute("");

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
		return super.onOptionsItemSelected(item);
	}

	@Override
	public void onTabSelected(ActionBar.Tab tab,
			FragmentTransaction fragmentTransaction) {
		// When the given tab is selected, switch to the corresponding page in
		// the ViewPager.
		mViewPager.setCurrentItem(tab.getPosition());
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
			if (getArguments().getInt(ARG_SECTION_NUMBER) == 1){
				
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
				ArrayAdapter<String> adapter = new ArrayAdapter<String>(getActivity().getBaseContext(),
						android.R.layout.simple_list_item_1, android.R.id.text1, list_values);
				// Assign adapter to ListView
	            listView.setAdapter(adapter); 
	            
	            // ListView Item Click Listener
	            listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
	 
	                  @Override
	                  public void onItemClick(AdapterView<?> parent, View view,
	                     int position, long id) {
	                    
	                   // ListView Clicked item index
	                   int itemPosition     = position;
	                   
	                   // ListView Clicked item value
	                   String  itemValue    = (String) listView.getItemAtPosition(position);
	                      
	                    // Show Alert 
	                    Toast.makeText(getActivity().getBaseContext(),
	                      "Position :"+itemPosition+"  ListItem : " +itemValue , Toast.LENGTH_LONG)
	                      .show();
	                 
	                  }
	    
	             }); 
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
	
	public class ConnectTask extends AsyncTask<String,String,TCPClient> {
	    	 
        @Override
        protected TCPClient doInBackground(String... message) {
            //we create a TCPClient object and
            mTcpClient = new TCPClient(new TCPClient.OnMessageReceived() {
                @Override
                //here the messageReceived method is implemented
                public void messageReceived(String message) {
                    //this method calls the onProgressUpdate
                    publishProgress(message);
                }
            });
            mTcpClient.run();
            return null;
        }
 
        @Override
        protected void onProgressUpdate(String... values) {
            super.onProgressUpdate(values);

            ArrayList<String> list = new ArrayList<String>();
            XmlPullParserFactory factory;
            StringBuilder s = null;
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
		        		  if(eventType == XmlPullParser.TEXT)
		        			  s.append("Postcode:");
		        			  s.append(xpp.getText());
		        	  }	
		        	  else if (xpp.getName().equalsIgnoreCase("JOB_DESC")){
		        		  eventType = xpp.next();
		        		  if(eventType == XmlPullParser.TEXT)
		        		  	  s.append(" ");
		        			  s.append(xpp.getText());
		        	  }
		        	  else if (xpp.getName().equalsIgnoreCase("NEED")){
		        		  eventType = xpp.next();
		        		  if(eventType == XmlPullParser.TEXT)
		        		  	  s.append(" Needs:");
		        			  s.append(xpp.getText());
		        	  }	
		        	  else if (xpp.getName().equalsIgnoreCase("JOB_START_TIME")){
		        		  eventType = xpp.next();
		        		  if(eventType == XmlPullParser.TEXT)
		        		  	  s.append(" Date-Time:");
		        			  s.append(xpp.getText());
		        	  }
		        	  else if (xpp.getName().equalsIgnoreCase("JOB_DURATION")){
		        		  eventType = xpp.next();
		        		  if(eventType == XmlPullParser.TEXT)
		        		  	  s.append(" for ");
		        			  s.append(xpp.getText());
		        	  }	
		          } else if(eventType == XmlPullParser.TEXT) {
		          } else if(eventType == XmlPullParser.END_TAG) {
		        	  if (xpp.getName().equalsIgnoreCase("JOB")){
		        		  list.add(s.toString());
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
			list_values = list.toArray(new String[list.size()]);
            mSectionsPagerAdapter.notifyDataSetChanged();
        }
	}	// end Class ConnectTask
}

