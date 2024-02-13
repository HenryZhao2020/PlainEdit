#pragma once

/**
 * @brief Contains GUI utilities for the Windows OS.
 */
namespace WinGui {
    /**
     * @brief Raises the specified widget to the front.
     * @param widget The widget to be raised.
     */
    void raiseWidget(QWidget *widget);

    /**
     * @brief Checks if the OS is in dark mode.
     * @return true if the OS is in dark mode; false otherwise.
     */
    bool isDarkMode();

    /**
     * @brief Relaunches the program.
     */
    void relaunch();
}
