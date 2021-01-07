package com.google.android.exoplayer2;

import com.google.android.exoplayer2.text.ssa.SsaSubtitle;
import com.google.android.exoplayer2.util.Log;
import java.lang.reflect.Constructor;
import java.lang.reflect.Method;

public class CceObject {
    public static CceObject inst = new CceObject();

    public boolean AUS_SUBTITLE = false;

    private Object lib;

    public CceObject() {
        Log.i("LUCI_CCE", "CceObject construction is called");
        try {
            Class<?> clazz = Class.forName("com.google.android.exoplayer2.ext.cce.CceLibrary");
            Constructor<?> constructor = clazz.getConstructor();
            lib = (Object) constructor.newInstance();
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (Exception e) {
            throw new RuntimeException("Error instantiating CCE extension", e);
        }
    }

    public void init() {
        try {
            Class<?> clazz = Class.forName("com.google.android.exoplayer2.ext.cce.CceLibrary");
            Method m = clazz.getMethod("init");
            m.invoke(lib);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (Exception e) {
            throw new RuntimeException("Error instantiating CCE extension", e);
        }
    }

    public void write(byte[] bytes, int length) {
        try {
            Class<?> clazz = Class.forName("com.google.android.exoplayer2.ext.cce.CceLibrary");
            Method m = clazz.getMethod("write", byte[].class, int.class);
            m.invoke(lib, bytes, length);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (Exception e) {
            throw new RuntimeException("Error instantiating CCE extension", e);
        }
    }

    public void run(long inputLength) {
        try {
            Class<?> clazz = Class.forName("com.google.android.exoplayer2.ext.cce.CceLibrary");
            Method m = clazz.getMethod("run", long.class);
            m.invoke(lib, inputLength);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (Exception e) {
            throw new RuntimeException("Error instantiating CCE extension", e);
        }
    }

    public void finish() {
        try {
            Class<?> clazz = Class.forName("com.google.android.exoplayer2.ext.cce.CceLibrary");
            Method m = clazz.getMethod("finish");
            m.invoke(lib);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (Exception e) {
            throw new RuntimeException("Error instantiating CCE extension", e);
        }
    }

    public byte[] readHeader() {
        try {
            Class<?> clazz = Class.forName("com.google.android.exoplayer2.ext.cce.CceLibrary");
            Method m = clazz.getMethod("readHeader");
            Object value =  m.invoke(lib);
            return (byte[]) value;
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (Exception e) {
            throw new RuntimeException("Error instantiating CCE extension", e);
        }
        return null;
    }

    public void finishedHeader() {
        try {
            Class<?> clazz = Class.forName("com.google.android.exoplayer2.ext.cce.CceLibrary");
            Method m = clazz.getMethod("finishedHeader");
            m.invoke(lib);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (Exception e) {
            throw new RuntimeException("Error instantiating CCE extension", e);
        }
    }

    public void setSubtitle(SsaSubtitle subtitle) {
        try {
            Class<?> clazz = Class.forName("com.google.android.exoplayer2.ext.cce.CceLibrary");
            Method m = clazz.getMethod("setSubtitle", SsaSubtitle.class);
            m.invoke(lib, subtitle);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (Exception e) {
            throw new RuntimeException("Error instantiating CCE extension", e);
        }
    }
}
