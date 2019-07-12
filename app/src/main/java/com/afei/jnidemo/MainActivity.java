package com.afei.jnidemo;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity implements View.OnClickListener{

    public static final String TAG = "MainActivity";

    private Button mGetDataBtn;
    private Button mSetDataBtn;

    private DataBean mDataBean;

    @Override
    protected void
    onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initView();
    }

    private void initView() {
        mGetDataBtn = findViewById(R.id.get_btn);
        mSetDataBtn = findViewById(R.id.set_btn);
        mGetDataBtn.setOnClickListener(this);
        mSetDataBtn.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.get_btn:
                mDataBean = NativeLibrary.getDataFromNative();
                Log.d(TAG, "getDataFromNative: " + mDataBean);
                break;
            case R.id.set_btn:
                mDataBean.mInner.mMessage = "data from java";
                Log.d(TAG, "transferDataToNative: " + mDataBean);
                NativeLibrary.transferDataToNative(mDataBean);
                break;
        }
    }
}
