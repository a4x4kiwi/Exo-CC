# Australia Subtitle How To #

## Based on Exoplayer 

I made the module named “extension-cce”, which is for getting the subtitle from Australian Channels.
I inserted this module in “extension” folder.

By making this module, you will be able to insert my engine easily after updating the ExoPlayer.

This module returns subtitles as SSA format.

## How to insert my engine after ExoPlayer updating:

* Step 1>  Import “extension-cce” module in your Exoplayer project.
* Step 2>  I made new Java class named “CceObject” for using this module.
* Step 3>  Insert some code that use CceObject into three classes of “core” library for getting subtitles.

  * 3.1>  In TsExtractor class (com.android.google.exoplayer2.ts.TsExtractor)
```
    public void initCCExtractor() {								
      CceObject.inst.init();								
      lengthForLib = 0;									
      firstPacket = true;									
    }						
```

```
public boolean sniff(ExtractorInput input) throws IOException, InterruptedException {		
  ...										
										
  if (isSyncBytePatternCorrect) {							
    ...										
    CceObject.inst.AUS_SUBTITLE = true;							
    return true;									
  }										
										
  ...										
}										
```

```
public void release() {								
  CceObject.inst.finish();								
}  										

```

```
public @ReadResult int read(ExtractorInput input, PositionHolder seekPosition)			
  throws IOException, InterruptedException {						
  ...										
										
  System.arraycopy(tsPacketBuffer.data,tsPacketBuffer.getPosition(),bytesForLib,lengthForLib,188);	
  lengthForLib += 188;								
										
  if (lengthForLib == 1880) {								
    CceObject.inst.write(bytesForLib, lengthForLib);						
										
    if (firstPacket == true) {								
      CceObject.inst.run(inputLength);							
      firstPacket = false;								
    }										
										
    lengthForLib = 0;									
  }										
										
  ...										
}										

```

  * 3.2>  In SingleSampleMediaPeriod 
(com.android.google.exoplayer2.source.SingleSampleMediaPeriod)

```
public void load() throws IOException, InterruptedException {					
  ...										
										
  if (CceObject.inst.AUS_SUBTITLE || dataSpec.uri.toString().equals(AUS_SUB_URL)) {			
    byte[] bytes = CceObject.inst.readHeader();						
										
    Arrays.fill(sampleData, (byte)0);							
										
    System.arraycopy(bytes, 0, sampleData, 0, bytes.length);					
  }										
										
  ...										
}										
```

 * 3.3>  In SsaDecoder (com.android.google.exoplayer2.text.ssa.SsaDecoder)

```
protected Subtitle decode(byte[] bytes, int length, boolean reset) {				
  ...										
										
  if (CceObject.inst.AUS_SUBTITLE) {							
    subtitle.decoder = this;								
    CceObject.inst.setSubtitle(subtitle);							
    CceObject.inst.finishedHeader();							
  }										
										
  ...										
}										
```

* Step 4: Add the “extension-cce” module in dependencies of gradle. (build.gradle of project)

```
dependencies {									
  ...										
										
  implementation project(modulePrefix + 'extension-cce')					
										
  ...										
}										
```

## Why the demo project is modified:

I have solved the problem what the subtitles often flicker.
As you can see if you try it with the original ExoPlayer, the subtitles often flicker.
The reason of the subtitles flickering often is that they keep redrawing each time the subtitles change. 
For solving this problem, I used TextView control in demo project instead of original SubtitleView of ExoPlayer library.
Also changed dispatchDraw function of SubtitleView class so that the subtitles are not displayed.

```
public void dispatchDraw(Canvas canvas) {							
  if (CceObject.inst.AUS_SUBTITLE)							
    return;									
										
  ...										
}										
```

If you would like to watch the original subtitle view, not TextView, please remove above two lines and set “visibility” attribute of TextView to “invisible” in demo project.


Why subtitles can’t be displayed with the correct color and position:

I implemented for displaying the subtitles with correct color but to do this, I modified the library code a lot.
Unfortunately, you don’t want to modify the library code a lot, except Australian Subtitles part.

Reason #1:

As mentioned above, CCExtractor engine returns the subtitles as SSA format.
Also, ExoPlayer supports SSA(Sub Station Alpha) subtitle file format. Subtitles Information (Font Name, Font Size, Text Color ...) are included in header.

e.x) SSA HEADER EXAMPLE
...
[V4+ Styles]
Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, OutlineColour, BackColour, Bold, Italic, Underline, StrikeOut, ScaleX, ScaleY, Spacing, Angle, BorderStyle, Outline, Shadow, Alignment, MarginL, MarginR, MarginV, Encoding
Style: Default,Arial,20,&H0000FFFF,&H000000FF,&H00000000,&H00000000,0,0,0,0,100,100,0,0,1,1,1,2,10,10,10,0
...

In this example, font name is Arial, font size is 20px and text color is #00FFFF(yellow).
Alignment is 2 and alignment reflects the position of subtitles. 2 is BOTTOM CENTER.
...

Like this, the SSA header has all the information about subtitles.

However, the handling for the header of this format is not accurate. 
 

Above function (fromFromatLine) parses the format info from a “Format:” line in the [V4+ Styles] section.
As you see, this function gets only the indices of style name and alignment. 
 
And then using the indices of style name and alignment obtained above, ExoPlayer engine gets the value of style name and alignment. (In above example, style name is “Default”, alignment is 2(BOTTOM CENTER))
This engine doesn’t get the text color, font name, font size ...
So we can’t display the subtitles with correct color and correct position.
Of course, I am able to display the subtitles with correct color and correct position but to do this I should modify a lot of ExoPlayer library and you don’t want it now.

If you want it, I will modify the code a lot.


Reason #2:

CCExtractor engine always return the same header information.
In other words, this engine returns the correct text color of each subtitles but this always returns the alignment as 2. So I can’t get correct alignment using this engine.
I hope you will understand only this part.


============================
Below is from original Exo.

===========================================================================================================================================================
# ExoPlayer #

ExoPlayer is an application level media player for Android. It provides an
alternative to Android’s MediaPlayer API for playing audio and video both
locally and over the Internet. ExoPlayer supports features not currently
supported by Android’s MediaPlayer API, including DASH and SmoothStreaming
adaptive playbacks. Unlike the MediaPlayer API, ExoPlayer is easy to customize
and extend, and can be updated through Play Store application updates.

## Documentation ##

* The [developer guide][] provides a wealth of information.
* The [class reference][] documents ExoPlayer classes.
* The [release notes][] document the major changes in each release.
* Follow our [developer blog][] to keep up to date with the latest ExoPlayer
  developments!

[developer guide]: https://exoplayer.dev/guide.html
[class reference]: https://exoplayer.dev/doc/reference
[release notes]: https://github.com/google/ExoPlayer/blob/release-v2/RELEASENOTES.md
[developer blog]: https://medium.com/google-exoplayer

## Using ExoPlayer ##

ExoPlayer modules can be obtained from JCenter. It's also possible to clone the
repository and depend on the modules locally.

### From JCenter ###

#### 1. Add repositories ####

The easiest way to get started using ExoPlayer is to add it as a gradle
dependency. You need to make sure you have the Google and JCenter repositories
included in the `build.gradle` file in the root of your project:

```gradle
repositories {
    google()
    jcenter()
}
```

#### 2. Add ExoPlayer module dependencies ####

Next add a dependency in the `build.gradle` file of your app module. The
following will add a dependency to the full library:

```gradle
implementation 'com.google.android.exoplayer:exoplayer:2.X.X'
```

where `2.X.X` is your preferred version.

As an alternative to the full library, you can depend on only the library
modules that you actually need. For example the following will add dependencies
on the Core, DASH and UI library modules, as might be required for an app that
plays DASH content:

```gradle
implementation 'com.google.android.exoplayer:exoplayer-core:2.X.X'
implementation 'com.google.android.exoplayer:exoplayer-dash:2.X.X'
implementation 'com.google.android.exoplayer:exoplayer-ui:2.X.X'
```

The available library modules are listed below. Adding a dependency to the full
library is equivalent to adding dependencies on all of the library modules
individually.

* `exoplayer-core`: Core functionality (required).
* `exoplayer-dash`: Support for DASH content.
* `exoplayer-hls`: Support for HLS content.
* `exoplayer-smoothstreaming`: Support for SmoothStreaming content.
* `exoplayer-ui`: UI components and resources for use with ExoPlayer.

In addition to library modules, ExoPlayer has multiple extension modules that
depend on external libraries to provide additional functionality. Some
extensions are available from JCenter, whereas others must be built manually.
Browse the [extensions directory][] and their individual READMEs for details.

More information on the library and extension modules that are available from
JCenter can be found on [Bintray][].

[extensions directory]: https://github.com/google/ExoPlayer/tree/release-v2/extensions/
[Bintray]: https://bintray.com/google/exoplayer

#### 3. Turn on Java 8 support ####

If not enabled already, you also need to turn on Java 8 support in all
`build.gradle` files depending on ExoPlayer, by adding the following to the
`android` section:

```gradle
compileOptions {
  targetCompatibility JavaVersion.VERSION_1_8
}
```

### Locally ###

Cloning the repository and depending on the modules locally is required when
using some ExoPlayer extension modules. It's also a suitable approach if you
want to make local changes to ExoPlayer, or if you want to use a development
branch.

First, clone the repository into a local directory and checkout the desired
branch:

```sh
git clone https://github.com/google/ExoPlayer.git
cd ExoPlayer
git checkout release-v2
```

Next, add the following to your project's `settings.gradle` file, replacing
`path/to/exoplayer` with the path to your local copy:

```gradle
gradle.ext.exoplayerRoot = 'path/to/exoplayer'
gradle.ext.exoplayerModulePrefix = 'exoplayer-'
apply from: new File(gradle.ext.exoplayerRoot, 'core_settings.gradle')
```

You should now see the ExoPlayer modules appear as part of your project. You can
depend on them as you would on any other local module, for example:

```gradle
implementation project(':exoplayer-library-core')
implementation project(':exoplayer-library-dash')
implementation project(':exoplayer-library-ui')
```

## Developing ExoPlayer ##

#### Project branches ####

* Development work happens on the `dev-v2` branch. Pull requests should
  normally be made to this branch.
* The `release-v2` branch holds the most recent release.

#### Using Android Studio ####

To develop ExoPlayer using Android Studio, simply open the ExoPlayer project in
the root directory of the repository.
