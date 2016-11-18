package org.opm.openpackagemanager;

/**
 * Created by bhavesh on 16/11/16.
 */

public class Package {
    private String name;
    private String version;
    private boolean updateAvailable;

    public Package() {
    }

    public Package(String name, String version, boolean updateAvailable) {
        this.name = name;
        this.version = version;
        this.updateAvailable = updateAvailable;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getVersion() {
        return version;
    }

    public boolean getUpdateAvailable() {
        return updateAvailable;
    }

    public void setVersion(String version) {
        this.version = version;
    }

    public void setUpdateAvailable(boolean available) {
        this.updateAvailable = updateAvailable;
    }

}