/*===================================================================

BlueBerry Platform

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#ifndef BERRYPLUGINACTIVATOR_H
#define BERRYPLUGINACTIVATOR_H

#include <ctkPluginActivator.h>

namespace berry {

class org_blueberry_core_jobs_Activator :
  public QObject, public ctkPluginActivator
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org_blueberry_core_jobs")
  Q_INTERFACES(ctkPluginActivator)

public:
  org_blueberry_core_jobs_Activator();

  void start(ctkPluginContext* context) override;
  void stop(ctkPluginContext* context) override;
};

typedef org_blueberry_core_jobs_Activator PluginActivator;

}

#endif // BERRYPLUGINACTIVATOR_H
