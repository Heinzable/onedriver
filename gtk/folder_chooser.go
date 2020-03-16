// +build linux,cgo

package gtk

/*
#cgo linux pkg-config: gtk+-3.0
#include <stdlib.h>
#include "folder_chooser.h"
*/
import "C"

import (
	"unsafe"
)

// FolderChooser creates a folder chooser window via GTK.
func FolderChooser(title string) string {
	windowTitle := C.CString(title)
	result := C.folder_chooser(windowTitle)
	defer C.free(unsafe.Pointer(windowTitle))
	defer C.free(unsafe.Pointer(result))
	return C.GoString(result)
}
