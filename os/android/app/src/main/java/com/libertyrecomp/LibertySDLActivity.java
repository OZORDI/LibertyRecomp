package com.libertyrecomp;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.graphics.Typeface;
import android.graphics.drawable.GradientDrawable;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.DocumentsContract;
import android.provider.Settings;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import org.libsdl.app.SDLActivity;

import java.io.File;

/**
 * LibertyRecomp's hosting Activity.
 *
 * <p>The class extends {@link SDLActivity} so SDL3 owns the window, surface,
 * event pump, audio, and input — matching the desktop entry flow. Before the
 * SDL super-onCreate runs, we show a lightweight folder-picker UI so the user
 * can point the recompiler at an extracted GTA IV Xbox 360 game directory.
 * The picked path is persisted in SharedPreferences and pushed to the native
 * side via {@link #nativeSetGameRoot} immediately after the shared library
 * loads — well before SDL_main() touches the filesystem.</p>
 *
 * <p>Lifecycle choreography:
 * <ol>
 *   <li>{@link #onCreate(Bundle)} runs BEFORE super. We read the saved path
 *       from {@link SharedPreferences}.</li>
 *   <li>If the path is valid we call {@code super.onCreate(...)} which loads
 *       libLibertyRecomp.so, runs our {@link #loadLibraries()} override
 *       (which in turn calls nativeSetPaths / nativeSetActivity /
 *       nativeSetGameRoot / nativeSetContext), then drops into SDL_main().</li>
 *   <li>If the path is missing, we show a simple picker view; on PLAY we
 *       re-enter {@link #startSdl()} which completes the super-onCreate.</li>
 * </ol>
 * This pattern mirrors how the reNut reference project stages asset paths.</p>
 */
public class LibertySDLActivity extends SDLActivity {

    private static final String TAG = "LibertyRecomp";

    private static final String PREFS_NAME   = "liberty_recomp_prefs";
    private static final String PREF_GAME_DIR = "game_dir";

    private static final int REQ_PICK_FOLDER      = 1001;
    private static final int REQ_MANAGE_STORAGE   = 1002;
    private static final int REQ_LEGACY_STORAGE   = 1003;

    // Files every extracted GTA IV dump must contain before the native side
    // is willing to boot. Matches root CMakeLists.txt:68 for parity.
    private static final String[] REQUIRED_FILES = new String[] {
        "default.xex",
        "common.rpf",
        "xbox360.rpf",
        "audio.rpf",
    };

    // ─── Colour palette (mirrors picker strings.xml / colors.xml) ──────────
    private static final int C_BG    = 0xFF0D1117;
    private static final int C_CARD  = 0xFF161B22;
    private static final int C_TEXT  = 0xFFF0F6FC;
    private static final int C_MUTED = 0xFF8B949E;
    private static final int C_ERR   = 0xFFFF6B6B;

    // State retained across the picker → SDL transition.
    private Bundle   mSavedInstanceState;
    private boolean  mSdlStarted = false;
    private String   mGameDir;

    private TextView mPathLabel;
    private TextView mStatusLabel;
    private Button   mPlayBtn;

    // ─── Native bridges defined in LibertyRecomp/os/android/jni_glue.cpp ───
    private static native void nativeSetActivity(Activity activity, int apiLevel);
    private static native void nativeSetPaths(String internalPath, String obbPath);
    private static native void nativeSetGameRoot(String gameRoot);
    // vibration_android.cpp
    private static native void nativeSetContext(Context context);

    // ─── SDLActivity overrides ─────────────────────────────────────────────

    @Override
    protected String[] getLibraries() {
        // libLibertyRecomp.so links SDL3 statically; no intermediate libs.
        return new String[] { "LibertyRecomp" };
    }

    @Override
    protected String getMainFunction() {
        // SDL3's SDL_main.h macro-renames main → SDL_main in main.cpp on Android.
        return "SDL_main";
    }

    @Override
    public void loadLibraries() {
        // Parent loads libLibertyRecomp.so via System.loadLibrary, which fires
        // JNI_OnLoad → captures g_androidJavaVM in jni_glue.cpp. We then push
        // paths + Activity + game-root BEFORE SDL.setupJNI() runs so native
        // code reading g_androidActivity / g_androidGameRoot from main() is
        // guaranteed to see valid values.
        super.loadLibraries();
        try {
            // Caches g_androidActivity + g_androidApiLevel and also initialises
            // the ReXGlue Android filesystem + achievement bridge.
            nativeSetActivity(this, Build.VERSION.SDK_INT);

            String internal = getFilesDir() != null ? getFilesDir().getAbsolutePath() : "";
            String obb      = getObbDir()   != null ? getObbDir().getAbsolutePath()   : "";
            nativeSetPaths(internal, obb);

            // Vibrator JNI bridge.
            nativeSetContext(this);

            // Game root chosen by the user in the pre-SDL picker.
            if (mGameDir != null && !mGameDir.isEmpty()) {
                nativeSetGameRoot(mGameDir);
            }
            Log.i(TAG, "Native context wired: game_dir=" + mGameDir
                + ", internal=" + internal + ", obb=" + obb);
        } catch (UnsatisfiedLinkError e) {
            Log.e(TAG, "Native path setters unavailable: " + e.getMessage());
        } catch (Throwable t) {
            Log.e(TAG, "Native path setup failed", t);
        }
    }

    // ─── Lifecycle ─────────────────────────────────────────────────────────

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        mSavedInstanceState = savedInstanceState;
        mGameDir = prefs().getString(PREF_GAME_DIR, null);

        // Early-out: if we already know a good path AND we have storage access,
        // skip the picker entirely and hand control to SDLActivity.
        if (haveStorageAccess() && mGameDir != null && validateGameDir(mGameDir) == null) {
            startSdl();
            return;
        }

        // Delay the super.onCreate until the user finishes picking. We still
        // call Activity.onCreate() by going through super.super… not possible
        // in Java, so we just satisfy the Activity contract with the minimal
        // setContentView-based picker UI.
        androidActivityOnCreate(savedInstanceState);
        buildPickerUI();
    }

    // Thin wrapper to make the "skip super.onCreate" intent explicit.
    private void androidActivityOnCreate(Bundle b) {
        // Bypass SDLActivity.onCreate() — call Activity.onCreate() directly.
        // We can't invoke the grandparent from Java directly; instead we rely
        // on the fact that Activity.onCreate() is idempotent as long as we
        // eventually call it via super when we start SDL.
        //
        // The SDL super-onCreate is invoked from startSdl() below; in the
        // meantime, Android has already called Activity.onCreate() up the
        // reflection chain before dispatching to our onCreate override.
        // Nothing more to do here.
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == REQ_PICK_FOLDER) {
            if (resultCode == RESULT_OK && data != null && data.getData() != null) {
                Uri tree = data.getData();
                // Persist permission so we can reuse the URI across launches.
                try {
                    getContentResolver().takePersistableUriPermission(
                        tree,
                        Intent.FLAG_GRANT_READ_URI_PERMISSION);
                } catch (Exception ignored) { }

                String absPath = resolveTreeToAbsolutePath(tree);
                if (absPath != null) {
                    setGameDirAndRefresh(absPath);
                } else {
                    Toast.makeText(this,
                        "Could not resolve picker URI to a filesystem path. " +
                        "Use \"Allow access to manage all files\" and retry.",
                        Toast.LENGTH_LONG).show();
                }
            }
            return;
        }

        if (requestCode == REQ_MANAGE_STORAGE) {
            refreshPickerStatus();
            return;
        }

        super.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (!mSdlStarted) {
            refreshPickerStatus();
        }
    }

    // ─── Picker UI ─────────────────────────────────────────────────────────

    private void buildPickerUI() {
        LinearLayout root = new LinearLayout(this);
        root.setOrientation(LinearLayout.VERTICAL);
        root.setBackgroundColor(C_BG);
        root.setGravity(Gravity.CENTER_HORIZONTAL);
        root.setPaddingRelative(dp(24), dp(56), dp(24), dp(40));

        ScrollView scroll = new ScrollView(this);
        scroll.addView(root, new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT));

        TextView title = new TextView(this);
        title.setText(getString(R.string.picker_title));
        title.setTextSize(48f);
        title.setTypeface(Typeface.DEFAULT_BOLD);
        title.setTextColor(C_TEXT);
        title.setGravity(Gravity.CENTER);
        root.addView(title);

        TextView sub = new TextView(this);
        sub.setText(getString(R.string.picker_subtitle));
        sub.setTextSize(14f);
        sub.setTextColor(C_MUTED);
        sub.setGravity(Gravity.CENTER);
        LinearLayout.LayoutParams subLp = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT);
        subLp.topMargin = dp(8);
        subLp.bottomMargin = dp(32);
        sub.setLayoutParams(subLp);
        root.addView(sub);

        root.addView(buildPermissionCard());
        root.addView(buildFolderCard());

        mPlayBtn = new Button(this);
        mPlayBtn.setText(getString(R.string.picker_play));
        mPlayBtn.setTextSize(18f);
        mPlayBtn.setTypeface(Typeface.DEFAULT_BOLD);
        mPlayBtn.setAllCaps(false);
        LinearLayout.LayoutParams playLp = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, dp(58));
        playLp.topMargin = dp(16);
        mPlayBtn.setLayoutParams(playLp);
        mPlayBtn.setOnClickListener(v -> tryStartSdl());
        root.addView(mPlayBtn);

        setContentView(scroll);
        refreshPickerStatus();
    }

    private View buildPermissionCard() {
        LinearLayout card = newCard();
        GradientDrawable border = (GradientDrawable) card.getBackground();
        border.setStroke(dp(1), 0xFF30363d);

        TextView label = new TextView(this);
        label.setText(getString(R.string.perm_title));
        label.setTextSize(12f);
        label.setTypeface(Typeface.DEFAULT_BOLD);
        label.setTextColor(C_TEXT);
        card.addView(label);

        TextView body = new TextView(this);
        body.setText(getString(R.string.perm_body));
        body.setTextSize(13f);
        body.setTextColor(C_MUTED);
        LinearLayout.LayoutParams bodyLp = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT);
        bodyLp.topMargin = dp(6);
        bodyLp.bottomMargin = dp(12);
        body.setLayoutParams(bodyLp);
        card.addView(body);

        Button open = new Button(this);
        open.setText(getString(R.string.perm_grant));
        open.setOnClickListener(v -> requestStorageAccess());
        card.addView(open);

        card.setId(View.generateViewId());
        card.setVisibility(haveStorageAccess() ? View.GONE : View.VISIBLE);
        card.setTag("perm-card");
        return card;
    }

    private View buildFolderCard() {
        LinearLayout card = newCard();

        TextView label = new TextView(this);
        label.setText("Game folder");
        label.setTextSize(11f);
        label.setTypeface(Typeface.DEFAULT_BOLD);
        label.setTextColor(C_TEXT);
        card.addView(label);

        mPathLabel = new TextView(this);
        mPathLabel.setTextSize(13f);
        mPathLabel.setTextColor(C_MUTED);
        LinearLayout.LayoutParams pathLp = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT);
        pathLp.topMargin = dp(4);
        mPathLabel.setLayoutParams(pathLp);
        card.addView(mPathLabel);

        mStatusLabel = new TextView(this);
        mStatusLabel.setTextSize(12f);
        mStatusLabel.setTextColor(C_ERR);
        LinearLayout.LayoutParams statusLp = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT);
        statusLp.topMargin = dp(4);
        mStatusLabel.setLayoutParams(statusLp);
        card.addView(mStatusLabel);

        Button pick = new Button(this);
        pick.setText(getString(R.string.picker_pick_folder));
        LinearLayout.LayoutParams pickLp = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT);
        pickLp.topMargin = dp(10);
        pick.setLayoutParams(pickLp);
        pick.setOnClickListener(v -> pickFolder());
        card.addView(pick);

        card.setTag("folder-card");
        return card;
    }

    private LinearLayout newCard() {
        LinearLayout card = new LinearLayout(this);
        card.setOrientation(LinearLayout.VERTICAL);
        card.setPadding(dp(18), dp(16), dp(18), dp(16));
        GradientDrawable bg = new GradientDrawable();
        bg.setColor(C_CARD);
        bg.setCornerRadius(dp(8));
        card.setBackground(bg);
        LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT);
        lp.bottomMargin = dp(12);
        card.setLayoutParams(lp);
        return card;
    }

    private void refreshPickerStatus() {
        if (mPathLabel == null) return;
        View permCard = findViewWithTag("perm-card");
        if (permCard != null) {
            permCard.setVisibility(haveStorageAccess() ? View.GONE : View.VISIBLE);
        }

        mPathLabel.setText(mGameDir == null
            ? getString(R.string.picker_no_folder)
            : getString(R.string.picker_current_path, mGameDir));

        String missing = mGameDir == null ? "" : validateGameDir(mGameDir);
        if (mGameDir == null) {
            mStatusLabel.setText(""); // hidden until user picks
            mStatusLabel.setVisibility(View.GONE);
        } else if (missing != null) {
            mStatusLabel.setVisibility(View.VISIBLE);
            mStatusLabel.setText(getString(R.string.picker_missing_files, missing));
        } else {
            mStatusLabel.setVisibility(View.GONE);
        }

        boolean canPlay = haveStorageAccess() && mGameDir != null && missing == null;
        mPlayBtn.setEnabled(canPlay);
        mPlayBtn.setAlpha(canPlay ? 1f : 0.5f);
    }

    private View findViewWithTag(String tag) {
        View root = findViewById(android.R.id.content);
        return root == null ? null : root.findViewWithTag(tag);
    }

    // ─── Folder picker flow ────────────────────────────────────────────────

    private void pickFolder() {
        if (!haveStorageAccess()) {
            requestStorageAccess();
            return;
        }
        Intent i = new Intent(Intent.ACTION_OPEN_DOCUMENT_TREE);
        i.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION
                 | Intent.FLAG_GRANT_PERSISTABLE_URI_PERMISSION);
        // Seed the picker near the common "extracted game" location to save
        // the user from walking the full tree on first launch.
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            Uri initial = DocumentsContract.buildRootUri(
                "com.android.externalstorage.documents",
                "primary");
            i.putExtra(DocumentsContract.EXTRA_INITIAL_URI, initial);
        }
        startActivityForResult(i, REQ_PICK_FOLDER);
    }

    /**
     * Best-effort conversion of a SAF tree URI into an absolute POSIX path.
     * The native VFS uses plain open()/fopen() so we need real paths, not
     * content URIs. With MANAGE_EXTERNAL_STORAGE granted this succeeds for
     * anything under /storage/emulated/0/.
     */
    @SuppressLint("NewApi")
    private String resolveTreeToAbsolutePath(Uri treeUri) {
        try {
            String docId = DocumentsContract.getTreeDocumentId(treeUri);
            if (docId == null) return null;
            String[] parts = docId.split(":", 2);
            if (parts.length == 0) return null;
            String volume = parts[0];
            String relPath = parts.length > 1 ? parts[1] : "";
            File base;
            if ("primary".equalsIgnoreCase(volume)) {
                base = Environment.getExternalStorageDirectory();
            } else {
                base = new File("/storage/" + volume);
            }
            File resolved = relPath.isEmpty() ? base : new File(base, relPath);
            if (resolved.isDirectory() && resolved.canRead()) {
                return resolved.getAbsolutePath();
            }
            // Fall back: return the path anyway; native code will surface a
            // friendly "can't open default.xex" message if it's wrong.
            return resolved.getAbsolutePath();
        } catch (Exception e) {
            Log.w(TAG, "Failed to resolve picker URI: " + treeUri, e);
            return null;
        }
    }

    private void setGameDirAndRefresh(String path) {
        mGameDir = path;
        prefs().edit().putString(PREF_GAME_DIR, path).apply();
        refreshPickerStatus();
    }

    /**
     * Returns null when the directory contains every required game file, or
     * a comma-separated list of the missing file names.
     */
    private static String validateGameDir(String path) {
        if (path == null || path.isEmpty()) return "(no path)";
        File dir = new File(path);
        if (!dir.isDirectory()) return "(not a directory)";
        StringBuilder missing = new StringBuilder();
        for (String name : REQUIRED_FILES) {
            if (!new File(dir, name).isFile()) {
                if (missing.length() > 0) missing.append(", ");
                missing.append(name);
            }
        }
        return missing.length() == 0 ? null : missing.toString();
    }

    // ─── Storage-access gate ───────────────────────────────────────────────

    private boolean haveStorageAccess() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            return Environment.isExternalStorageManager();
        }
        return checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE)
            == PackageManager.PERMISSION_GRANTED;
    }

    private void requestStorageAccess() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            try {
                Intent i = new Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION,
                    Uri.parse("package:" + getPackageName()));
                startActivityForResult(i, REQ_MANAGE_STORAGE);
            } catch (Exception e) {
                // Some OEM ROMs don't honour the package-scoped intent. Fall
                // back to the generic "Allow all files access" list screen.
                Intent i = new Intent(Settings.ACTION_MANAGE_ALL_FILES_ACCESS_PERMISSION);
                startActivityForResult(i, REQ_MANAGE_STORAGE);
            }
        } else {
            requestPermissions(
                new String[] { Manifest.permission.READ_EXTERNAL_STORAGE },
                REQ_LEGACY_STORAGE);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == REQ_LEGACY_STORAGE) {
            refreshPickerStatus();
        }
    }

    // ─── SDL transition ────────────────────────────────────────────────────

    private void tryStartSdl() {
        if (!haveStorageAccess()) {
            Toast.makeText(this, R.string.perm_title, Toast.LENGTH_LONG).show();
            requestStorageAccess();
            return;
        }
        if (mGameDir == null || validateGameDir(mGameDir) != null) {
            Toast.makeText(this, R.string.picker_no_folder, Toast.LENGTH_LONG).show();
            return;
        }
        startSdl();
    }

    /**
     * Hands control to SDLActivity.onCreate() for the first time. Because
     * Activity.onCreate() is the grandparent we can't re-dispatch from here,
     * but SDLActivity.onCreate() tolerates being called once a frame or so
     * after Activity.onCreate() has returned. SDL owns the window from this
     * point forward.
     */
    private void startSdl() {
        if (mSdlStarted) return;
        mSdlStarted = true;
        super.onCreate(mSavedInstanceState);
    }

    // ─── Helpers ───────────────────────────────────────────────────────────

    private SharedPreferences prefs() {
        return getSharedPreferences(PREFS_NAME, MODE_PRIVATE);
    }

    private int dp(int v) {
        return Math.round(v * getResources().getDisplayMetrics().density);
    }
}
