if(MITK_USE_OpenCL)

  # create the module
  MITK_CREATE_MODULE(
    DEPENDS MitkCore
    PACKAGE_DEPENDS
      PUBLIC OpenCL
      PRIVATE ITK|ITKThresholding
  )

  add_subdirectory(Testing)
endif(MITK_USE_OpenCL)
