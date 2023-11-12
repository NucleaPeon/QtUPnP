#ifndef STDACTION_HPP
#define STDACTION_HPP

#include <upnp/controlpoint.hpp>
#include <QDialog>

namespace Ui
{
  class CStdAction;
}

namespace QtUPnP
{
  class CControlPoint;
}

class QGridLayout;

USING_UPNP_NAMESPACE

/*! \brief The dialog to enter in parameters, launch an action and display out parameters. */
class CStdAction : public QDialog
{
  Q_OBJECT

public:
  explicit CStdAction (CControlPoint* cp, QString const & deviceUUID,
           QString const & action, QWidget* parent = NULL);
  ~CStdAction ();

protected slots :
  void on_m_invoke_clicked ();
  void on_m_close_clicked ();

private :
  void createWidgets (QString const & actionName);

private:
  Ui::CStdAction* ui = NULL;
  QGridLayout* m_inLayout = NULL, * m_outLayout = NULL;
  CControlPoint* m_cp = NULL;
  QString m_deviceUUID;
  QString m_serviceID;
  QString m_actionName;
  QList<CControlPoint::TArgValue> m_args;
};

#endif // STDACTION_HPP
