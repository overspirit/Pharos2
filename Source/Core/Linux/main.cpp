#include <gtk/gtk.h>
#include "PreCompile.h"
#include "Pharos.h"

string OpenFileDialog(const char* defaultPath)
{    
	string openFilePath;

	gtk_init(NULL, NULL);

    int file_return;
    GtkWidget *file =gtk_file_chooser_dialog_new("SelectFile",NULL,GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,GTK_STOCK_OK,GTK_RESPONSE_ACCEPT,NULL);
    if(gtk_dialog_run(GTK_DIALOG(file))==GTK_RESPONSE_ACCEPT)
	{
		openFilePath = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file));
		//printf(filename);
	}		

	//g_signal_connect(G_OBJECT(file),"destroy",G_CALLBACK(gtk_main_quit),NULL);

	gtk_widget_destroy(file);
	
	while(gtk_events_pending())
	{
		gtk_main_iteration();
	}

	return openFilePath;
}

int main(int argc, char *argv[])
{
	if (!sPlatform->Init()) return -1;

	int32 ret = sPlatform->Run();

	sPlatform->Destroy();

	return ret;
}
