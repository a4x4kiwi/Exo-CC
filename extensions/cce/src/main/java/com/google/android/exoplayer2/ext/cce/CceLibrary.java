package com.google.android.exoplayer2.ext.cce;

import android.util.Log;

import com.google.android.exoplayer2.text.ssa.SsaSubtitle;
import com.google.android.exoplayer2.util.ParsableByteArray;

public class CceLibrary extends Object {


    public static String TAG = "TS_CCELIBRARY";

    static {
        try {
            System.loadLibrary("ccejni");
        }
        catch(UnsatisfiedLinkError e) {
        }
    }

    private static SsaSubtitle subtitle;

    private String logFolderPath;

    public CceLibrary() {
        logFolderPath = null;
    }

    public void init() {
        jniInitCcextractor(null/*logFolderPath + "/exo_jni.log"*/);
    }

    public void run(long inputLength) {
        jniRunCcextractor(inputLength);
    }

    public void write(byte[] bytes, int length) {
        jniWriteSharedBytes(bytes, length);
    }

    public byte[] readHeader() { return jniGetSsaHeader(); }

    public void finishedHeader() {
        jniFinishedHeader();
    }

    public void setSubtitle(SsaSubtitle subtitle) { CceLibrary.subtitle = subtitle; }

    public void finish() {
        jniEndCcextractor();
    }

    public void appendSubtitle(byte[] data) {
        Log.i(TAG, "Append subti    tles: " + new String(data));
        CceLibrary.subtitle.addSubtitles(new ParsableByteArray(data));
    }

    private native void jniInitCcextractor(String logFolderPath);

    private native void jniRunCcextractor(long length);

    private native void jniWriteSharedBytes(byte[] bytes, int length);

    private native byte[] jniGetSsaHeader();

    private native void jniEndCcextractor();

    private native void jniFinishedHeader();
}