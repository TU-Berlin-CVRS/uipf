MetaData LoadImageModule::getMetaData() const {
	map<string, DataDescription> output = {
		{"image", DataDescription(MATRIX, "loaded image.") }
	};
	map<string, ParamDescription> params = {
		{"filename", ParamDescription("file name ...") },
		{"mode", ParamDescription("can be either 'color' or 'grayscale'... Defaults to 'color'.",true) },
	};
	return MetaData(
		"Load an image from a file.",
		"I/O",
		map<string, DataDescription>(), // no inputs
		output,
		params
	);
}
