package org.hermesinstaller.scenes;

import javafx.event.ActionEvent;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.TextArea;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import org.hermesinstaller.InstallerEnvironment;
import org.hermesinstaller.PredefinedUIControls;

public class License extends Scene {

    private final TextArea license = new TextArea();

    public License(GridPane grid) {
        super(grid);

        //1st, 2nd and 3rd row
        grid.add(PredefinedUIControls.environmentInfo(), 1, 0);
        grid.add(PredefinedUIControls.rootWarning(), 1, 1);
        grid.add(PredefinedUIControls.decorateAsTitle("Hermes License"), 0, 2);

        //4th row
        license.setEditable(false);
        license.setWrapText(true);
        //license.setPrefColumnCount(20);
        license.prefRowCountProperty().bind(license.prefColumnCountProperty().add(license.prefColumnCountProperty().divide(6)));

        grid.add(license, 0, 3, 2, 1);

        //5th row
        Button back = new Button("Back");
        back.setOnAction((ActionEvent e) -> {
            InstallerEnvironment.primaryStage.setScene(InstallerEnvironment.versionScene);
        });

        Button accept = new Button("Accept");
        accept.setOnAction((ActionEvent e) -> {
            InstallerEnvironment.primaryStage.setScene(InstallerEnvironment.configurationScene);
        });

        HBox buttonHBox = new HBox(10);
        buttonHBox.setAlignment(Pos.BOTTOM_RIGHT);
        buttonHBox.getChildren().addAll(back, accept);

        grid.add(buttonHBox, 1, 4);
    }

    public void setLicenseText(String licenseText) {
        license.setText(licenseText);
    }
}
