#if defined(TGL_SHADER_NAME) && defined(TGL_SHADER_FILE)
template <typename TGLshape>
inline TGLcolor TGL_SHADER_NAME(TGLpoint &point, TGLshape &shape) {
    #include TGL_SHADER_FILE
}
#undef TGL_SHADER_NAME
#undef TGL_SHADER_FILE
#endif