#include "mainwindow.hpp"
#include "session.hpp"
#include "playlist.hpp"
#include "helper.hpp"
#include "ui_mainwindow.h"
#include "aivwidgets/networkprogress.hpp"
#include "aivwidgets/widgethelper.hpp"
#include "../upnp/avtransport.hpp"
#include <QMenu>

USING_UPNP_NAMESPACE

void CMainWindow::updateContentDirectory (CFolderItem const & item, bool parentID)
{
  QApplication::setOverrideCursor (Qt::WaitCursor);
  QString id = parentID ? item.parentID () : item.id ();
  if (!id.isEmpty ())
  {
    ui->m_contentDirectory->addItems (m_server, id);
  }

  item.setListWidgetCurrentItem ();
  QApplication::restoreOverrideCursor ();
}

void CMainWindow::muteIcon (bool mute)
{
  char const * iconName = mute ? "speaker_off" : "speaker_on";
  QIcon        icon     = ::resIcon (iconName);
  ui->m_mute->setIcon (icon);
  ui->m_mute2->setIcon (icon);
}

void CMainWindow::playingIcon (bool playing)
{
  char const * iconName = playing ? "playing_on" : "playing_off";
  QIcon        icon     = ::resIcon (iconName);
  ui->m_play->setIcon (icon);
  ui->m_play2->setIcon (icon);
}

void CMainWindow::updateVolumeSlider (int volume, bool blocked)
{
  if (blocked)
  {
    ui->m_volume->blockSignals (blocked);
    ui->m_volume->blockSignals (blocked);
  }

  ui->m_volume->setValue (volume);
  ui->m_volume2->setValue (volume);

  if (blocked)
  {
    ui->m_volume->blockSignals (false);
    ui->m_volume->blockSignals (false);
  }
}

void CMainWindow::updateCurrentPlayling (CDidlItem const & didl,
                   bool startPlaying, bool fullUpdate)
{
  QString title = didl.title ();
  if (fullUpdate || !title.isEmpty ())
  {
    ui->m_title->setText (title);
    ui->m_title->setToolTip (title);

    QStringList texts;
    texts << title;

    QString album = didl.album ();
    if (!album.isEmpty ())
    {
      texts << album;
    }

    QString artist = didl.artist ();
    if (artist.isEmpty ())
    {
      artist = didl.albumArtist ();
    }

    if (!artist.isEmpty ())
    {
      texts << artist;
    }

    char const * iconName = isFavorite (m_playlists, didl) ? "favorite_true" : "favorite";
    ui->m_favorite->setIcon (::resIcon (iconName));

    ui->m_trackMetadata->setText (texts);
    if (startPlaying)
    {
      startPositionTimer ();
    }
  }

  QString albumArtURI = didl.albumArtURI (0);
  if (fullUpdate || !albumArtURI.isEmpty ())
  {
    ui->m_cover->setImage (albumArtURI);
    ui->m_currentPlayingIcon->setIcon (QIcon (ui->m_cover->image ()));
  }
}

void CMainWindow::clearCurrentPlayling ()
{
  ui->m_title->clear ();
  ui->m_title->setToolTip (QString::null);

  ui->m_cover->setImage (QString::null);
  ui->m_currentPlayingIcon->setIcon (QIcon (ui->m_cover->image ()));
  ui->m_trackMetadata->clear ();
}

void CMainWindow::updateCurrentPlayling (CContentDirectoryBrowserItem const * item,
                     bool startPlaying, bool fullUpdate)
{
  CDidlItem const & didl = item->didlItem ();
  updateCurrentPlayling (didl, startPlaying, fullUpdate);
}

void CMainWindow::startPositionTimer ()
{
  if (!m_positionTimer.isActive ())
  {
    m_positionTimer.start ();
  }
}

void CMainWindow::stopPositionTimer ()
{
  if (m_positionTimer.isActive ())
  {
    m_positionTimer.stop ();
  }
}

void CMainWindow::togglePositionTimer (bool playing)
{
  if (playing)
  {
    startPositionTimer ();
  }
  else
  {
    stopPositionTimer ();
  }
}

void CMainWindow::setItemBold (QString const & uri)
{
  if (!uri.isEmpty ())
  {
    ui->m_contentDirectory->setBold (uri);
    ui->m_queue->setBold (uri);
  }
}

void CMainWindow::setItemBold (CContentDirectoryBrowserItem const * item)
{
  setItemBold (item->didlItem ());
}

void CMainWindow::setItemBold (CDidlItem const & item)
{
  QString const & uri = item.uri (0);
  setItemBold (uri);
}

void CMainWindow::setAVTransport (CContentDirectoryBrowserItem const * item)
{
  stopPositionTimer ();
  ui->m_queue->setAVTransportURI (m_cp, m_renderer, item->listWidget ()->row (item));
  setItemBold (item);
  playingIcon (true);
  updateCurrentPlayling (item, true, true);
  QApplication::restoreOverrideCursor ();
}

void CMainWindow::nextItem (bool forward)
{
  int index = ui->m_queue->nextIndex (forward, m_playMode == RepeatAll,
                                      m_playMode == Shuffle || m_playMode == Random);
  if (index != -1)
  {
    QListWidgetItem* item = ui->m_queue->item (index);
    on_m_queue_itemDoubleClicked (item);
  }
}

void CMainWindow::applyPlayMode ()
{
  char const * repeatIconName  = "repeat_off";
  char const * shuffleIconName = "shuffle_off";
  switch (m_playMode )
  {
    case RepeatOne :
      repeatIconName = "repeat_one";
      break;

    case RepeatAll :
      repeatIconName = "repeat_all";
      break;

    case Shuffle :
    case Random :
      shuffleIconName = "shuffle_on";
      break;

    default :
      break;
  }

  ui->m_repeat->setIcon (::resIcon (repeatIconName));
  ui->m_shuffle->setIcon (::resIcon (shuffleIconName));
}

QString CMainWindow::playModeString ()
{
  QString playModeList[] = { "NORMAL",
                             "REPEAT_ALL",
                             "REPEAT_ONE",
                             "SHUFFLE",
                             "RANDOM",
                           };

  CDevice const &             device          = m_cp->device (m_renderer);
  CStateVariable const &      currentPlayMode = device.stateVariable ("CurrentPlayMode", QString::null);
  QStringList const &         playModes       = currentPlayMode.allowedValues ();
  QStringList::const_iterator begin           = playModes.cbegin ();
  QStringList::const_iterator end             = playModes.cend ();

  QMap<int, QString> map;
  for (int iPlayMode = Normal; iPlayMode < LastPlayMode; ++iPlayMode)
  {
    int validPlayMode = iPlayMode;
    if (std::find (begin, end, playModeList[iPlayMode]) == end)
    {
      if (iPlayMode == Shuffle && std::find (begin, end, playModeList[Random]) != end)
      {
        validPlayMode = Random;
      }
      else if (iPlayMode == Random && std::find (begin, end, playModeList[Random]) != end)
      {
        validPlayMode = Shuffle;
      }
      else if (iPlayMode == RepeatOne && std::find (begin, end, playModeList[RepeatOne]) == end)
      {
        validPlayMode = Normal;
      }
      else if (iPlayMode == RepeatAll && std::find (begin, end, playModeList[RepeatAll]) == end)
      {
        validPlayMode = Normal;
      }
      else
      {
        validPlayMode = Normal;
      }
    }

    map.insert (iPlayMode, playModeList[validPlayMode]);
  }

  return map.value (m_playMode);
}

CMainWindow::EPlayMode CMainWindow::playMode (QString const & currentPlayModeString)
{
  EPlayMode currentPlayMode = Normal;
  QString   playModes []    = { "NORMAL",
                                "REPEAT_ALL",
                                "REPEAT_ONE",
                                "SHUFFLE",
                                "RANDOM",
                              };


  for (unsigned iPlayMode = 0; iPlayMode < sizeof (playModes) / sizeof (QString); ++iPlayMode)
  {
    if (playModes[iPlayMode] == currentPlayModeString)
    {
      currentPlayMode = static_cast<EPlayMode>(iPlayMode);
      break;
    }
  }

  return currentPlayMode;
}

void CMainWindow::applyLastSession ()
{
  CSession session;
  if (session.restore ())
  {
    session.setGeometry (this);
    m_renderer = session.renderer ();
    m_language  = session.language ();
    m_status.setStatus (session.status ());
  }

  m_status.addStatus (ShowNetworkCom);
  if (m_status.hasStatus (ShowNetworkCom))
  {
    CNetworkProgress* m_controlPointRenderer = new CNetworkProgress ("m_controlPointRenderer", this);
    ui->m_comNetworkLayout->insertWidget (2, m_controlPointRenderer);

    CNetworkProgress* m_controlPointServer = new CNetworkProgress ("m_controlPointServer", this);
    m_controlPointServer->setInverted (true);
    ui->m_comNetworkLayout->insertWidget (1, m_controlPointServer);
    m_cp->validateNetworkCom (true);
  }
  else
  {
    delete ui->m_comRenderer;
    delete ui->m_comServer;
    delete ui->m_comControlPoint;
  }

  m_playMode = static_cast<EPlayMode>(session.playMode ());
  applyPlayMode ();
  restorePlaylists (m_playlists);
  for (TMPlaylists::const_iterator it = m_playlists.cbegin (), end = m_playlists.cend (); it != end; ++it)
  {
    CPlaylist const & playlist = it.value ();
    CPlaylist::EType  type     = playlist.type ();
    if (type != CPlaylist::AudioFavorites && type != CPlaylist::ImageFavorites && type != CPlaylist::VideoFavorites)
    {
      ui->m_myDevice->addItem (type, it.key ());
    }
  }

  m_translator = installTranslator (m_language);

  typedef QPair<EStatus, CMyDeviceBrowserItem::EType> TCollapse;
  TCollapse collapses[] = { TCollapse (CollapseAudioPlaylist, CMyDeviceBrowserItem::AudioContainer),
                            TCollapse (CollapseImagePlaylist, CMyDeviceBrowserItem::ImageContainer),
                            TCollapse (CollapseVideoPlaylist, CMyDeviceBrowserItem::VideoContainer),
                          };

  for (TCollapse const & collapse : collapses)
  {
    if (m_status.hasStatus (collapse.first))
    {
      ui->m_myDevice->setCollapsed (collapse.second, true);
    }
  }

  updatePlaylistItemCount ();
}

void CMainWindow::rotateIcon ()
{
  int angle = m_iconAngle;
  if (angle == 30)
  {
    m_iconAngle = -15;
  }

  m_iconAngle += 15;
  QString iconName ("discovery");
  iconName += QString::number (angle);
  ui->m_home->setIcon (::resIcon (iconName));
}

int CMainWindow::didlItems (QList<CDidlItem>& items, CDidlItem const & item)
{
  if (item.isContainer ())
  {
    QString const &          id = item.containerID ();
    CContentDirectory        cd (m_cp);
    CBrowseReply             reply      = cd.browse (m_server, id);
    QList<CDidlItem> const & replyItems = reply.items ();
    for (CDidlItem const & replyItem : replyItems)
    {
      if (replyItem.isContainer ())
      {
        QString text = tr ("Scan: ") + replyItem.title ();
        ui->m_provider->setText (text);
      }

      didlItems (items, replyItem);
    }
  }
  else
  {
    items.push_back (item);
  }

  return items.size ();
}

QList<CDidlItem> CMainWindow::didlItems (QList<QListWidgetItem*> lwItems)
{
  QGuiApplication::setOverrideCursor (Qt::WaitCursor);
  int              count = lwItems.size () + 30;
  QList<CDidlItem> items;
  items.reserve (count);
  for (QListWidgetItem* lwItem : lwItems)
  {
    CContentDirectoryBrowserItem* cdItem = static_cast<CContentDirectoryBrowserItem*>(lwItem);
    didlItems (items, cdItem->didlItem ());
  }

  QGuiApplication::restoreOverrideCursor ();
  return items;
}

void CMainWindow::currentQueueChanged ()
{
  int index = ui->m_queue->boldIndex ();
  if (index < 0)
  {
    index = 0;
  }

  m_cp->clearPlaylist ();
  QListWidgetItem* item     = ui->m_queue->item (index);
  CPositionInfo    position = CAVTransport (m_cp).getPositionInfo (m_renderer);
  on_m_queue_itemDoubleClicked (item);
  QString relTime = position.relTime ();
  on_m_position_valueChanged (::timeToMS (relTime));
}

void CMainWindow::updatePlaylistItemCount ()
{
  ui->m_myDevice->updateContainerItemCount ();
  for (TMPlaylists::const_iterator it = m_playlists.begin (), end = m_playlists.end (); it != end; ++it)
  {
    CPlaylist const & playlist = it.value ();
    CPlaylist::EType  type     = playlist.type ();
    int               count    = playlist.items ().size ();
    if (type == CPlaylist::AudioFavorites || type == CPlaylist::ImageFavorites || type == CPlaylist::VideoFavorites)
    {
      ui->m_myDevice->setCount (static_cast<CMyDeviceBrowserItem::EType>(type), count);
    }
    else
    {
      ui->m_myDevice->setCount (it.key (), count);
    }
  }
}

void CMainWindow::updateDevicesCount (int* cServers, int* cRenderers)
{
  int cServersTmp, cRenderersTmp;
  if (cServers == nullptr)
  {
    cServers = &cServersTmp;
  }

  if (cRenderers == nullptr)
  {
    cRenderers = &cRenderersTmp;
  }

  if (ui->m_stackedWidget->currentIndex () == Home)
  {
    *cServers    = m_cp->serversCount ();
    *cRenderers  = m_cp->renderersCount ();
    QString text = QString::number (*cServers) + ' ' +
                   (*cServers <= 1 ? tr ("Server") : tr ("Servers")) +
                   tr (" and ") + QString::number (*cRenderers) + ' ' +
                   (*cRenderers <= 1 ? tr ("Renderer") : tr ("Renderers"));
    ui->m_provider->setText (text);
  }
}

void CMainWindow::searchAction (bool activate)
{
  static QString providerText;
  if (activate)
  {
    providerText = ui->m_provider->text ();
    ui->m_provider->clear ();
    ui->m_provider->setReadOnly (false);
    ui->m_provider->setFocus ();
  }
  else if (!ui->m_provider->isReadOnly ())
  {
    ui->m_provider->setReadOnly (true);
    ui->m_provider->setText (providerText);
  }
}

CNetworkProgress* CMainWindow::networkProgress (CDevice::EType type)
{
  QString name = type == CDevice::MediaRenderer ? "m_controlPointRenderer" : "m_controlPointServer";
  return ui->m_networkProgress->findChild<CNetworkProgress*> (name);
}

void CMainWindow::setComRendererIcon ()
{
  QIcon                icon;
  QMenu*               menu    = ui->m_renderer->menu ();
  QList<QAction*>      actions = menu->actions ();
  for (QAction const * action : actions)
  {
    QString renderer = action->data ().toString ();
    if (renderer == m_renderer)
    {
      icon = action->icon ();
      break;
    }
  }

  QPixmap pxm = icon.pixmap (ui->m_comRenderer->size ());
  if (pxm.isNull ())
  {
    pxm = QPixmap (::resIconFullPath ("device"));
  }

  ui->m_comRenderer->setPixmap (pxm);
}

void CMainWindow::setComServerIcon ()
{
  QPixmap pxm = ui->m_servers->icon (m_server).pixmap (ui->m_comServer->size ());
  if (pxm.isNull())
  {
    pxm = QPixmap (::resIconFullPath ("server"));
  }

  ui->m_comServer->setPixmap (pxm);
}