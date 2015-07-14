MetaData ResizeModule::getMetaData() const
{
    DataDescriptionMap input = {{"image", DataDescription(MATRIX, "the image to resize.") }	};

    DataDescriptionMap output = {{"image", DataDescription(MATRIX, "the result image.") }};

    ParamDescriptionMap params = {
        {"width", ParamDescription("new width") },
        {"height", ParamDescription("new height") }
    };

    return MetaData("Resizes an image using openCV.","Image Processing",input,output,params	);
}
