package org.hermesinstaller;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URL;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.nio.file.StandardOpenOption;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Properties;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.beans.property.SimpleStringProperty;
import javafx.stage.Stage;
import org.hermesinstaller.scenes.Configuration;
import org.hermesinstaller.scenes.License;
import org.hermesinstaller.scenes.Operation;
import org.hermesinstaller.scenes.PostgreSQL;
import org.hermesinstaller.scenes.Update;
import org.hermesinstaller.scenes.Version;

public class InstallerEnvironment {

    public static DateFormat isoDateFormat = new SimpleDateFormat("yyyy-MM-dd");
    public static final SimpleStringProperty installerDate = new SimpleStringProperty("2015-06-04");
    public static final SimpleStringProperty includedVersion = new SimpleStringProperty("1.8.5");
    public static final String releasesSite = "https://hermes-mod.java.net/Releases/";
    public static SimpleBooleanProperty isLatest = new SimpleBooleanProperty(false);
    public static String latestVersion;
    public static SimpleBooleanProperty installPrerequisites = new SimpleBooleanProperty(false);
    public static SimpleStringProperty osName = new SimpleStringProperty();
    public static SimpleStringProperty osArch = new SimpleStringProperty();
    public static SimpleStringProperty osUser = new SimpleStringProperty();
    public static SimpleStringProperty installAs = new SimpleStringProperty("Hermes");
    public static SimpleBooleanProperty plpython = new SimpleBooleanProperty(true);
    public static SimpleBooleanProperty postGIS = new SimpleBooleanProperty(true);
    public static SimpleStringProperty postgreSQLVersion = new SimpleStringProperty();
    public static SimpleStringProperty postgreSQLArch = new SimpleStringProperty();
    public static SimpleStringProperty postgreSQLInstallationPath = new SimpleStringProperty();
    public static SimpleStringProperty postgreSQLPKGLIBDIRPath = new SimpleStringProperty();
    public static SimpleStringProperty postgreSQLPort = new SimpleStringProperty();
    public static SimpleStringProperty postgreSQLPassword = new SimpleStringProperty();
    public static Boolean updateError;
    public static String[] dbnames;
    private static String dbversion;
    private static String[] dblibpath;
    private static String[] installedAs;
    private static byte[] library;
    private static String[] scripts;
    public static String updatedTo;
    //Scenes
    public static Stage primaryStage;
    public static Operation operationScene;
    public static Version versionScene;
    public static License licenseScene;
    public static Configuration configurationScene;
    public static PostgreSQL postgreSQLScene;
    public static Update updateScene;

    public static byte[] URLtoBytes(String url) {
        BufferedInputStream bufferedInputStream = null;
        try {
            bufferedInputStream = new BufferedInputStream(new URL(url.replace(" ", "%20")).openStream());

            ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
            byte[] buffer = new byte[4096];
            int n;

            while ((n = bufferedInputStream.read(buffer)) != -1) {
                if (n > 0) {
                    byteArrayOutputStream.write(buffer, 0, n);
                }
            }

            return byteArrayOutputStream.toByteArray();
        } catch (IOException ex) {
            Logger.getLogger(InstallerEnvironment.class.getName()).log(Level.SEVERE, null, ex);
        } finally {
            try {
                if (bufferedInputStream != null) {
                    bufferedInputStream.close();
                }
            } catch (IOException ex) {
                Logger.getLogger(InstallerEnvironment.class.getName()).log(Level.SEVERE, null, ex);
            }
        }

        return null;
    }

    public static String URLtoString(String url) {
        BufferedInputStream bufferedInputStream = null;
        try {
            bufferedInputStream = new BufferedInputStream(new URL(url.replace(" ", "%20")).openStream());

            ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
            byte[] buffer = new byte[4096];
            int n;

            while ((n = bufferedInputStream.read(buffer)) != -1) {
                if (n > 0) {
                    byteArrayOutputStream.write(buffer, 0, n);
                }
            }

            return byteArrayOutputStream.toString(Charset.forName("UTF-8").name());
        } catch (IOException ex) {
            Logger.getLogger(InstallerEnvironment.class.getName()).log(Level.SEVERE, null, ex);
        } finally {
            try {
                if (bufferedInputStream != null) {
                    bufferedInputStream.close();
                }
            } catch (IOException ex) {
                Logger.getLogger(InstallerEnvironment.class.getName()).log(Level.SEVERE, null, ex);
            }
        }

        return null;
    }

    public static void installPrerequisitesWindows() {
        try {
            switch (postgreSQLArch.get()) {
                case "32bit":
                    try {
                        Process process = Runtime.getRuntime().exec("Elevate\\x86\\elevate -wait \"Prerequisites\\Visual C++ Redistributable for Visual Studio\\vcredist_x86.exe\"");
                        process.waitFor();

                        process = Runtime.getRuntime().exec("msiexec /i Prerequisites\\Python\\python-3.3.5.msi");
                        process.waitFor();

                        process = Runtime.getRuntime().exec("Elevate\\x86\\elevate -wait \"Prerequisites\\PostgreSQL\\postgresql-9.4.3-1-windows.exe\"");
                        process.waitFor();
                    } catch (InterruptedException ex) {
                        Logger.getLogger(InstallerEnvironment.class.getName()).log(Level.SEVERE, null, ex);
                    }

                    break;
                case "64bit":
                    try {
                        Process process = Runtime.getRuntime().exec("Elevate\\amd64\\elevate -wait \"Prerequisites\\Visual C++ Redistributable for Visual Studio\\vcredist_x64.exe\"");
                        process.waitFor();

                        process = Runtime.getRuntime().exec("msiexec /i Prerequisites\\Python\\python-3.3.5.amd64.msi");
                        process.waitFor();

                        process = Runtime.getRuntime().exec("Elevate\\amd64\\elevate -wait \"Prerequisites\\PostgreSQL\\postgresql-9.4.3-1-windows-x64.exe\"");
                        process.waitFor();
                    } catch (InterruptedException ex) {
                        Logger.getLogger(InstallerEnvironment.class.getName()).log(Level.SEVERE, null, ex);
                    }

                    break;
            }
        } catch (IOException ex) {
            Logger.getLogger(InstallerEnvironment.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public static void findPostgreSQLInstallation() {
        try {
            String pg_config = null;
            Process process;
            boolean foundInstallation = false;
            for (int i = 0; i < 3 && !foundInstallation; i++) {
                switch (i) {
                    case 0:
                        if (osName.get().contains("Windows")) {
                            switch (postgreSQLArch.get()) {
                                case "32bit":
                                    pg_config = "C:\\Program Files (x86)\\PostgreSQL\\" + postgreSQLVersion.get() + "\\bin\\pg_config.exe";
                                    break;
                                case "64bit":
                                    pg_config = "C:\\Program Files\\PostgreSQL\\" + postgreSQLVersion.get() + "\\bin\\pg_config.exe";
                                    break;
                            }
                        } else if (osName.get().contains("Mac OS X")) {
                            pg_config = "/Library/PostgreSQL/" + postgreSQLVersion.get() + "/bin/pg_config";
                        } else {
                            pg_config = "/opt/PostgreSQL/" + postgreSQLVersion.get() + "/bin/pg_config";
                        }

                        break;
                    case 1:
                        if (osName.get().contains("Windows")) {
                            if (postgreSQLArch.get().equals("32bit")) {
                                pg_config = "C:\\Program Files\\PostgreSQL\\" + postgreSQLVersion.get() + "\\bin\\pg_config.exe";
                            }
                        } else {
                            pg_config = "/usr/lib/postgresql/" + postgreSQLVersion.get() + "/bin/pg_config";
                        }

                        break;
                    case 2:
                        if (osName.get().contains("Windows")) {
                            switch (postgreSQLArch.get()) {
                                case "32bit":
                                    pg_config = "C:\\PostgreSQL\\" + postgreSQLVersion.get() + "\\x86\\bin\\pg_config.exe";
                                    break;
                                case "64bit":
                                    pg_config = "C:\\PostgreSQL\\" + postgreSQLVersion.get() + "\\x64\\bin\\pg_config.exe";
                                    break;
                            }
                        } else {
                            pg_config = "/opt/PostgreSQL/pg0/bin/pg_config";
                        }

                        break;
                }

                try {
                    process = Runtime.getRuntime().exec(new String[]{pg_config, "--version"});
                    process.waitFor();
                    BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
                    String version = bufferedReader.readLine();

                    String arch;
                    if (osName.get().contains("Windows")) {
                        process = Runtime.getRuntime().exec(new String[]{"File\\file", pg_config});
                        process.waitFor();
                        bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
                        bufferedReader.readLine();
                        arch = bufferedReader.readLine();

                        if (arch.contains("64bit")) {
                            arch = "64bit";
                        } else if (arch.contains("32bit")) {
                            arch = "32bit";
                        }
                    } else if (InstallerEnvironment.osName.get().contains("Mac OS X")) {
                        process = Runtime.getRuntime().exec(new String[]{"file", pg_config});
                        process.waitFor();
                        bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));

                        boolean bit64 = false, bit32 = false;
                        while ((arch = bufferedReader.readLine()) != null) {
                            if (arch.contains("x86_64")) {
                                bit64 = true;
                            } else if (arch.contains("i386")) {
                                bit32 = true;
                            }
                        }

                        if (bit64) {
                            arch = "64bit";
                        } else if (bit32) {
                            arch = "32bit";
                        }
                    } else {
                        process = Runtime.getRuntime().exec(new String[]{"file", pg_config});
                        process.waitFor();
                        bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
                        arch = bufferedReader.readLine();

                        if (arch.contains("64-bit")) {
                            arch = "64bit";
                        } else if (arch.contains("32-bit")) {
                            arch = "32bit";
                        }
                    }

                    if (version.contains(postgreSQLVersion.get()) && arch.equals(postgreSQLArch.get())) {
                        foundInstallation = true;
                        break;
                    }
                } catch (IOException ex) {
                    //Logger.getLogger(InstallerEnvironment.class.getName()).log(Level.SEVERE, null, ex);
                }
            }

            if (foundInstallation) {
                process = Runtime.getRuntime().exec(new String[]{pg_config, "--bindir"});
                process.waitFor();
                BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
                String string = bufferedReader.readLine();
                postgreSQLInstallationPath.set(string.substring(0, string.lastIndexOf("/")));

                process = Runtime.getRuntime().exec(new String[]{pg_config, "--pkglibdir"});
                process.waitFor();
                bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
                postgreSQLPKGLIBDIRPath.set(bufferedReader.readLine());

                postgreSQLScene.updatePostgreSQLInstallation();
            }
        } catch (IOException | InterruptedException ex) {
            Logger.getLogger(InstallerEnvironment.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    private static boolean prepareInstall() {
        Properties properties = new Properties();
        try {
            if (isLatest.get()) {
                properties.load(new URL(releasesSite + latestVersion + "/scripts.properties").openStream());
            } else {
                properties.load(Files.newInputStream(Paths.get("Hermes/scripts.properties")));
            }
        } catch (IOException ex) {
            Logger.getLogger(InstallerEnvironment.class.getName()).log(Level.SEVERE, null, ex);
            return false;
        }

        String string = "Hermes/C Library/";
        if (osName.get().contains("Windows")) {
            switch (postgreSQLArch.get()) {
                case "32bit":
                    string += "Windows x86/Hermes.dll";
                    break;
                case "64bit":
                    string += "Windows amd64/Hermes.dll";
                    break;
            }
        } else if (osName.get().contains("Mac OS X")) {
            switch (postgreSQLArch.get()) {
                case "32bit":
                    string += "OS X Mach-O/Hermes.dylib";
                    break;
                case "64bit":
                    string += "OS X Mach-O/Hermes.dylib";
                    break;
            }
        } else {
            switch (postgreSQLArch.get()) {
                case "32bit":
                    string += "Linux x86/Hermes.so";
                    break;
                case "64bit":
                    string += "Linux amd64/Hermes.so";
                    break;
            }
        }

        if (isLatest.get()) {
            library = URLtoBytes(releasesSite + latestVersion + "/" + string);
        } else {
            try {
                library = Files.readAllBytes(Paths.get(string));
            } catch (IOException ex) {
                Logger.getLogger(InstallerEnvironment.class.getName()).log(Level.SEVERE, null, ex);
                return false;
            }
        }

        scripts = new String[properties.size()];

        for (int i = 1; i <= scripts.length; i++) {
            if (isLatest.get()) {
                scripts[i - 1] = URLtoString(releasesSite + latestVersion + "/" + properties.getProperty(String.valueOf(i)));
            } else {
                try {
                    scripts[i - 1] = new String(Files.readAllBytes(Paths.get(properties.getProperty(String.valueOf(i)))), Charset.forName("UTF-8"));
                } catch (IOException ex) {
                    Logger.getLogger(InstallerEnvironment.class.getName()).log(Level.SEVERE, null, ex);
                    return false;
                }
            }
        }

        return true;
    }

    public static boolean installHermes() {
        Connection conn;
        try {
            conn = DriverManager.getConnection("jdbc:postgresql://localhost:" + postgreSQLPort.get() + "/postgres", "postgres", postgreSQLPassword.get());
        } catch (SQLException ex) {
            PredefinedUIControls.showMessageDialog("The port or/and the password you gave is/are not valid!");

            Logger.getLogger(InstallerEnvironment.class.getName()).log(Level.SEVERE, null, ex);
            return false;
        }

        if (!prepareInstall()) {
            PredefinedUIControls.showMessageDialog("Could not load installation files.");

            return false;
        }

        try {
            Path lib = Paths.get(postgreSQLPKGLIBDIRPath.get() + File.separatorChar + (osName.get().contains("Windows") ? installAs.get() + ".dll" : osName.get().contains("Mac OS X") ? installAs.get() + ".dylib" : installAs.get() + ".so"));
            Path backup = Paths.get(postgreSQLPKGLIBDIRPath.get() + File.separatorChar + (osName.get().contains("Windows") ? installAs.get() + "Old.dll" : osName.get().contains("Mac OS X") ? installAs.get() + "Old.dylib" : installAs.get() + "Old.so"));

            if (Files.exists(lib)) {
                Files.copy(lib, backup, StandardCopyOption.REPLACE_EXISTING);
            }

            Files.write(lib, library, StandardOpenOption.CREATE, StandardOpenOption.WRITE, StandardOpenOption.TRUNCATE_EXISTING);
            //Files.setOwner(lib, lib.getFileSystem().getUserPrincipalLookupService().lookupPrincipalByName("postgres"));
        } catch (IOException ex) {
            PredefinedUIControls.showMessageDialog("PostgreSQL directory is not writable!");

            Logger.getLogger(InstallerEnvironment.class.getName()).log(Level.SEVERE, null, ex);
            return false;
        }

        try {
            Statement stmt = conn.createStatement();

            stmt.execute("DROP DATABASE IF EXISTS template_" + installAs.get().toLowerCase() + ";");
            stmt.execute("CREATE DATABASE template_" + installAs.get().toLowerCase() + " WITH ENCODING = 'UTF8';");

            stmt.close();
            conn.close();
        } catch (SQLException ex) {
            Logger.getLogger(InstallerEnvironment.class.getName()).log(Level.SEVERE, null, ex);
            return false;
        }

        try {
            conn = DriverManager.getConnection("jdbc:postgresql://localhost:" + postgreSQLPort.get() + "/template_" + installAs.get().toLowerCase(), "postgres", postgreSQLPassword.get());
            Statement stmt = conn.createStatement();

            for (String script : scripts) {
                if (osName.get().contains("Mac OS X")) {
                    stmt.execute(script.replace("'$libdir/Hermes'", "'$libdir/" + installAs.get() + ".dylib'"));
                } else {
                    stmt.execute(script.replace("'$libdir/Hermes'", "'$libdir/" + installAs.get() + "'"));
                }
            }

            if (plpython.get()) {
                stmt.execute("CREATE LANGUAGE plpython3u;");
            }

            if (postGIS.get()) {
                stmt.execute("CREATE EXTENSION postgis;");
                stmt.execute("SELECT HParameters('postgis_integration', 'true');");
            }

            stmt.execute("SELECT HParameters('hermes_installer_installed_as', '" + installAs.get() + "');");
            stmt.execute("SELECT HParameters('hermes_installer_libpath', '" + postgreSQLPKGLIBDIRPath.get() + "/" + (osName.get().contains("Windows") ? installAs.get() + ".dll" : osName.get().contains("Mac OS X") ? installAs.get() + ".dylib" : installAs.get() + ".so") + "');");

            stmt.close();
            conn.close();
        } catch (SQLException ex) {
            Logger.getLogger(InstallerEnvironment.class.getName()).log(Level.SEVERE, null, ex);
            return false;
        }

        return true;
    }

    private static boolean prepareUpdate() {
        updateError = true;

        Connection conn;
        Statement stmt;
        ResultSet rs;

        String dbversion_prev = null;
        try {
            for (int i = 0; i < dbnames.length; i++) {
                conn = DriverManager.getConnection("jdbc:postgresql://localhost:" + postgreSQLPort.get() + "/" + dbnames[i], "postgres", postgreSQLPassword.get());

                stmt = conn.createStatement();
                rs = stmt.executeQuery("SELECT HermesVersionString();");

                rs.next();
                dbversion = rs.getString(1);
                if (dbversion == null) {
                    rs.close();

                    return false;
                }
                rs.close();

                if (i > 0) {
                    if (!dbversion.equals(dbversion_prev)) {
                        PredefinedUIControls.showMessageDialog("Databases are not in the same version.");

                        return false;
                    }
                }

                rs = stmt.executeQuery("SELECT HParameters('hermes_installer_installed_as');");
                rs.next();
                installedAs[i] = rs.getString(1);
                if (installedAs[i] == null) {
                    PredefinedUIControls.showMessageDialog("Hermes was not initially installed via the installer.");

                    return false;
                }
                rs.close();

                rs = stmt.executeQuery("SELECT HParameters('hermes_installer_libpath');");
                rs.next();
                dblibpath[i] = rs.getString(1);
                if (dblibpath[i] == null) {
                    PredefinedUIControls.showMessageDialog("Hermes was not initially installed via the installer.");

                    return false;
                }
                rs.close();

                dbversion_prev = dbversion;

                stmt.close();
                conn.close();
            }
        } catch (SQLException ex) {
            PredefinedUIControls.showMessageDialog("Database object doesn't exist.");

            Logger.getLogger(InstallerEnvironment.class.getName()).log(Level.SEVERE, null, ex);
            return false;
        }

        try {
            Properties properties = new Properties();
            properties.load(new URL(releasesSite + "Updates/" + dbversion + "/installer.properties").openStream());

            String minimumCompatibleInstaller = properties.getProperty("Minimum Compatible Installer");
            String maximumCompatibleInstaller = properties.getProperty("Maximum Compatible Installer");
            if (isoDateFormat.parse(installerDate.get()).before(isoDateFormat.parse(minimumCompatibleInstaller))
                    || isoDateFormat.parse(installerDate.get()).after(isoDateFormat.parse(maximumCompatibleInstaller))) {

                PredefinedUIControls.showMessageDialog("This version of the installer is not compatible with this update.\nDownload the latest version of this installer.");

                return false;
            }
        } catch (ParseException | IOException ex) {
            Logger.getLogger(InstallerEnvironment.class.getName()).log(Level.SEVERE, null, ex);

            updateError = false;
            return false;
        }

        try {
            Process process;
            BufferedReader bufferedReader;

            String arch;
            if (osName.get().contains("Windows")) {
                process = Runtime.getRuntime().exec(new String[]{"File\\file", dblibpath[0]});
                process.waitFor();
                bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
                bufferedReader.readLine();
                arch = bufferedReader.readLine();

                if (arch.contains("32bit")) {
                    postgreSQLArch.set("32bit");
                } else if (arch.contains("64bit")) {
                    postgreSQLArch.set("64bit");
                }
            } else {
                process = Runtime.getRuntime().exec(new String[]{"file", dblibpath[0]});
                process.waitFor();
                bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
                arch = bufferedReader.readLine();

                if (arch.contains("32-bit")) {
                    postgreSQLArch.set("32bit");
                } else if (arch.contains("64-bit")) {
                    postgreSQLArch.set("64bit");
                }
            }
        } catch (InterruptedException | IOException ex) {
            Logger.getLogger(PostgreSQL.class.getName()).log(Level.SEVERE, null, ex);
            return false;
        }

        Properties properties = new Properties();
        try {
            properties.load(new URL(releasesSite + "Updates/" + dbversion + "/scripts.properties").openStream());
        } catch (IOException ex) {
            Logger.getLogger(InstallerEnvironment.class.getName()).log(Level.SEVERE, null, ex);
            return false;
        }

        scripts = new String[properties.size()];

        for (int i = 1; i <= scripts.length; i++) {
            scripts[i - 1] = URLtoString(releasesSite + "Updates/" + dbversion + "/" + properties.getProperty(String.valueOf(i)));
        }

        String string = "Hermes/C Library/";
        if (osName.get().contains("Windows")) {
            switch (postgreSQLArch.get()) {
                case "32bit":
                    string += "Windows x86/Hermes.dll";
                    break;
                case "64bit":
                    string += "Windows amd64/Hermes.dll";
                    break;
            }
        } else {
            switch (postgreSQLArch.get()) {
                case "32bit":
                    string += "Linux x86/Hermes.so";
                    break;
                case "64bit":
                    string += "Linux amd64/Hermes.so";
                    break;
            }
        }

        library = URLtoBytes(releasesSite + "Updates/" + dbversion + "/" + string);
        if (library == null) {
            return false;
        }

        updateError = false;
        return true;
    }

    public static boolean updateHermes() {
        Connection conn;
        try {
            conn = DriverManager.getConnection("jdbc:postgresql://localhost:" + postgreSQLPort.get() + "/postgres", "postgres", postgreSQLPassword.get());
        } catch (SQLException ex) {
            PredefinedUIControls.showMessageDialog("The port or/and the password you gave is/are not valid!");

            Logger.getLogger(InstallerEnvironment.class.getName()).log(Level.SEVERE, null, ex);
            return false;
        }

        dblibpath = new String[dbnames.length];
        installedAs = new String[dbnames.length];

        Statement stmt;
        while (prepareUpdate()) {
            try {
                for (String path : dblibpath) {
                    Files.write(Paths.get(path), library, StandardOpenOption.CREATE, StandardOpenOption.WRITE, StandardOpenOption.TRUNCATE_EXISTING);
                }
            } catch (IOException ex) {
                PredefinedUIControls.showMessageDialog("Could not write library!");

                Logger.getLogger(InstallerEnvironment.class.getName()).log(Level.SEVERE, null, ex);
                return false;
            }

            try {
                for (int i = 0; i < dbnames.length; i++) {
                    conn = DriverManager.getConnection("jdbc:postgresql://localhost:" + postgreSQLPort.get() + "/" + dbnames[i], "postgres", postgreSQLPassword.get());
                    stmt = conn.createStatement();

                    for (String script : scripts) {
                        stmt.execute(script.replace("'$libdir/Hermes'", "'$libdir/" + installedAs[i] + "'"));
                    }

                    stmt.close();
                    conn.close();
                }

                conn = DriverManager.getConnection("jdbc:postgresql://localhost:" + postgreSQLPort.get() + "/" + dbnames[0], "postgres", postgreSQLPassword.get());

                stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery("SELECT HermesVersionString();");

                rs.next();
                updatedTo = rs.getString(1);
                rs.close();

                stmt.close();
                conn.close();
            } catch (SQLException ex) {
                PredefinedUIControls.showMessageDialog("Could not run script!");

                Logger.getLogger(InstallerEnvironment.class.getName()).log(Level.SEVERE, null, ex);
                return false;
            }
        }

        return true;
    }
}
