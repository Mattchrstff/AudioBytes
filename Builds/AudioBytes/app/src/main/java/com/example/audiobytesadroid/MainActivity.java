package com.example.audiobytesadroid;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import com.example.audiobytesadroid.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("audiobytesadroid");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Now works after adding @+id/sampleText to XML
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());
    }

    public native String stringFromJNI();
}
