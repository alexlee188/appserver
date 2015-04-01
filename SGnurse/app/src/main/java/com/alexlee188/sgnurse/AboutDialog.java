package com.alexlee188.sgnurse;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.text.Html;
import android.text.util.Linkify;
import android.graphics.Color;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class AboutDialog extends Dialog{
	
private static Context mContext = null;
public AboutDialog(Context context) {
	super(context);
	mContext = context;
}

@Override
public void onCreate(Bundle savedInstanceState) {
	setContentView(R.layout.about);
	TextView tv = (TextView)findViewById(R.id.info_text);
	tv.setText(Html.fromHtml(readRawTextFile(R.raw.info)));
	tv.setLinkTextColor(Color.parseColor("#0099CC"));
	Linkify.addLinks(tv, Linkify.ALL);
    Button btn = (Button) findViewById(R.id.About_OK_btn);
    btn.setOnClickListener(new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            dismiss();
        }
    });
}
public static String readRawTextFile(int id) {
    InputStream inputStream = mContext.getResources().openRawResource(id);
    InputStreamReader in = new InputStreamReader(inputStream);
    BufferedReader buf = new BufferedReader(in);
    String line;
    StringBuilder text = new StringBuilder();
    try {
        while (( line = buf.readLine()) != null) text.append(line);
        } catch (IOException e) {
        return null;
    }
    return text.toString();
}
}
