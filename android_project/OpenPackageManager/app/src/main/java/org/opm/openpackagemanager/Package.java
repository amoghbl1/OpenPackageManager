package org.opm.openpackagemanager;

/**
 * Created by bhavesh on 16/11/16.
 */

public class Package {
    private String name;
    private String version;

    public Package() {
    }

    public Package(String name, String version) {
        this.name = name;
        this.version = version;
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

    public void setVersion(String version) {
        this.version = version;
    }
}