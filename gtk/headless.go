// +build !linux !cgo

package gtk

// FolderChooser normally would create a file picker window to select a folder,
// but we're evidently compiled in headless mode and that's not supported.
func FolderChooser(title string) string {
	return ""
}
