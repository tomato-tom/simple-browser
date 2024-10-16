#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

static void activate(GtkApplication* app, gpointer user_data);
static void destroyWindowCb(GtkWidget* widget, gpointer user_data);
static void goBackCb(GtkWidget* button, WebKitWebView* webView);
static void goForwardCb(GtkWidget* button, WebKitWebView* webView);
static void refreshCb(GtkWidget* button, WebKitWebView* webView);
static void loadUrlCb(GtkWidget* entry, WebKitWebView* webView);
static void updateNavigationButtons(WebKitWebView* webView, WebKitLoadEvent load_event, gpointer user_data);

typedef struct {
    GtkWidget *backButton;
    GtkWidget *forwardButton;
} NavigationButtons;

int main(int argc, char* argv[]) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.example.browser", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *headerBar;
    GtkWidget *backButton, *forwardButton, *refreshButton;
    GtkWidget *urlBar;
    GtkWidget *webView;
    GtkWidget *vbox;
    NavigationButtons *navButtons;

    // Create the main window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Simple Browser");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // Create header bar
    headerBar = gtk_header_bar_new();
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(headerBar), TRUE);
    gtk_header_bar_set_title(GTK_HEADER_BAR(headerBar), "Simple WebKit Browser");
    gtk_window_set_titlebar(GTK_WINDOW(window), headerBar);

    // Create navigation buttons
    backButton = gtk_button_new_from_icon_name("go-previous", GTK_ICON_SIZE_BUTTON);
    forwardButton = gtk_button_new_from_icon_name("go-next", GTK_ICON_SIZE_BUTTON);
    refreshButton = gtk_button_new_from_icon_name("view-refresh", GTK_ICON_SIZE_BUTTON);

    // Create URL bar
    urlBar = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(urlBar), "Enter URL");

    // Add buttons and URL bar to header bar
    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerBar), backButton);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerBar), forwardButton);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerBar), refreshButton);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerBar), urlBar);

    // Create a browser instance
    webView = webkit_web_view_new();

    // Create a vertical box to hold the web view
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(vbox), webView, TRUE, TRUE, 0);

    // Add the vertical box to the window
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Connect signals
    g_signal_connect(window, "destroy", G_CALLBACK(destroyWindowCb), app);
    g_signal_connect(backButton, "clicked", G_CALLBACK(goBackCb), webView);
    g_signal_connect(forwardButton, "clicked", G_CALLBACK(goForwardCb), webView);
    g_signal_connect(refreshButton, "clicked", G_CALLBACK(refreshCb), webView);
    g_signal_connect(urlBar, "activate", G_CALLBACK(loadUrlCb), webView);

    // Set up navigation buttons updating
    navButtons = g_new(NavigationButtons, 1);
    navButtons->backButton = backButton;
    navButtons->forwardButton = forwardButton;
    g_signal_connect(webView, "load-changed", G_CALLBACK(updateNavigationButtons), navButtons);

    // Load a default web page
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webView), "https://www.example.com");

    // Show all widgets
    gtk_widget_show_all(window);
}

static void destroyWindowCb(GtkWidget* widget, gpointer user_data) {
    GtkApplication *app = GTK_APPLICATION(user_data);
    g_application_quit(G_APPLICATION(app));
}

static void goBackCb(GtkWidget* button, WebKitWebView* webView) {
    webkit_web_view_go_back(webView);
}

static void goForwardCb(GtkWidget* button, WebKitWebView* webView) {
    webkit_web_view_go_forward(webView);
}

static void refreshCb(GtkWidget* button, WebKitWebView* webView) {
    webkit_web_view_reload(webView);
}

static void loadUrlCb(GtkWidget* entry, WebKitWebView* webView) {
    const gchar* url = gtk_entry_get_text(GTK_ENTRY(entry));
    webkit_web_view_load_uri(webView, url);
}

static void updateNavigationButtons(WebKitWebView* webView, WebKitLoadEvent load_event, gpointer user_data) {
    NavigationButtons *buttons = (NavigationButtons*)user_data;
    gtk_widget_set_sensitive(buttons->backButton, webkit_web_view_can_go_back(webView));
    gtk_widget_set_sensitive(buttons->forwardButton, webkit_web_view_can_go_forward(webView));
}
