/***************************************************************************
**                                                                        **
**  QCustomPlot, a simple to use, modern plotting widget for Qt           **
**  Copyright (C) 2011, 2012 Emanuel Eichhammer                           **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: http://www.WorksLikeClockwork.com/                   **
**             Date: 09.06.12                                             **
****************************************************************************/

/*! \file */

#include "core.h"

#include "painter.h"
#include "layer.h"
#include "axis.h"
#include "layoutelements/layoutelement-axisrect.h"
#include "layoutelements/layoutelement-legend.h"
#include "layoutelements/layoutelement-plottitle.h"
#include "plottable.h"
#include "plottables/plottable-graph.h"
#include "item.h"

/*! \mainpage %QCustomPlot Documentation
 
  Below is a brief overview of and guide to the classes and their relations. If you are new to
  QCustomPlot and just want to start using it, it's recommended to look at the examples/tutorials
  at
 
  http://www.WorksLikeClockWork.com/index.php/components/qt-plotting-widget
 
  This documentation is especially helpful when you're familiar with the basic concept of how to use
  %QCustomPlot and you wish to learn more about specific functionality.
 
  \section simpleoverview Simplified Class Overview
  
  \image latex ClassesOverviewSimplified.png "" width=1.2\textwidth
  \image html ClassesOverviewSimplified.png
  <center>Simplified diagram of most important classes, view the \ref classoverview "Class Overview" to see a full overview.</center>
  
  The central widget which displays the plottables and axes on its surface is QCustomPlot. Usually,
  you don't create the axes yourself, but you use the ones already inside every QCustomPlot
  instance (xAxis, yAxis, xAxis2, yAxis2).

  \section plottables Plottables
  
  \a Plottables are classes that display any kind of data inside the QCustomPlot. They all derive
  from QCPAbstractPlottable. For example, the QCPGraph class is a plottable that displays a graph
  inside the plot with different line styles, scatter styles, filling etc.
  
  Since plotting graphs is such a dominant use case, QCustomPlot has a special interface for working
  with QCPGraph plottables, that makes it very easy to handle them:\n
  You create a new graph with QCustomPlot::addGraph and access them with QCustomPlot::graph.
  
  For all other plottables, you need to use the normal plottable interface:\n
  First, you create an instance of the plottable you want, e.g.
  \code
  QCPCurve *newCurve = new QCPCurve(customPlot->xAxis, customPlot->yAxis);\endcode
  add it to the customPlot with QCustomPlot::addPlottable:
  \code
  customPlot->addPlottable(newCurve);\endcode
  and then modify the properties of the newly created plottable via <tt>newCurve</tt>.
  
  Plottables (including graphs) can be retrieved via QCustomPlot::plottable. Since the return type
  of that function is the abstract base class of all plottables, QCPAbstractPlottable, you will
  probably want to qobject_cast (or dynamic_cast) the returned pointer to the respective plottable
  subclass. (As usual, if the cast returns zero, the plottable wasn't of that specific subclass.)
  
  All further interfacing with plottables (e.g how to set data) is specific to the plottable type.
  See the documentations of the subclasses: QCPGraph, QCPCurve, QCPBars, QCPStatisticalBox.

  \section axes Controlling the Axes
  
  As mentioned, QCustomPlot has four axes by default: \a xAxis (bottom), \a yAxis (left), \a xAxis2
  (top), \a yAxis2 (right).
  
  Their range is handled by the simple QCPRange class. You can set the range with the
  QCPAxis::setRange function. By default, the axes represent a linear scale. To set a logarithmic
  scale, set QCPAxis::setScaleType to QCPAxis::stLogarithmic. The logarithm base can be set freely
  with QCPAxis::setScaleLogBase.
  
  By default, an axis automatically creates and labels ticks in a sensible manner, i.e. with a tick
  interval that's pleasing to the viewer. See the following functions for tick manipulation:\n
  QCPAxis::setTicks, QCPAxis::setAutoTicks, QCPAxis::setAutoTickCount, QCPAxis::setAutoTickStep,
  QCPAxis::setTickLabels, QCPAxis::setTickLabelType, QCPAxis::setTickLabelRotation,
  QCPAxis::setTickStep, QCPAxis::setTickLength,...
  
  Each axis can be given an axis label (e.g. "Voltage [mV]") with QCPAxis::setLabel.
  
  The distance of an axis backbone to the respective QCustomPlot widget border is called its margin.
  Normally, the margins are calculated automatically. To change this, set QCustomPlot::setAutoMargin
  to false and set the margins manually with QCustomPlot::setMargin.
  
  \section legend Plot Legend
  
  Every QCustomPlot owns a QCPLegend (as \a legend). That's a small window inside the plot which
  lists the plottables with an icon of the plottable line/symbol and a description. The Description
  is retrieved from the plottable name (QCPAbstractPlottable::setName). Plottables can be added and
  removed from the legend via \ref QCPAbstractPlottable::addToLegend and \ref
  QCPAbstractPlottable::removeFromLegend. By default, adding a plottable to QCustomPlot
  automatically adds it to the legend, too. This behaviour can be modified with the
  QCustomPlot::setAutoAddPlottableToLegend property.
  
  The QCPLegend provides an interface to access, add and remove legend items directly, too. See
  QCPLegend::item, QCPLegend::itemWithPlottable, QCPLegend::addItem, QCPLegend::removeItem for
  example.
  
  \section userinteraction User Interactions
  
  QCustomPlot currently supports dragging axis ranges with the mouse (\ref
  QCustomPlot::setRangeDrag), zooming axis ranges with the mouse wheel (\ref
  QCustomPlot::setRangeZoom) and a complete selection mechanism of most objects.
  
  The availability of these interactions is controlled with \ref QCustomPlot::setInteractions. For
  details about the interaction system, see the documentation there.
  
  Further, QCustomPlot always emits corresponding signals, when objects are clicked or
  doubleClicked. See \ref QCustomPlot::plottableClick, \ref QCustomPlot::plottableDoubleClick
  and \ref QCustomPlot::axisClick for example.
  
  \section items Items
  
  Apart from plottables there is another category of plot objects that are important: Items. The
  base class of all items is QCPAbstractItem. An item sets itself apart from plottables in that
  it's not necessarily bound to any axes. This means it may also be positioned in absolute pixel
  coordinates or placed at a relative position on the axis rect. Further, it usually doesn't
  represent data directly, but acts as decoration, emphasis, description etc.
  
  Multiple items can be arranged in a parent-child-hierarchy allowing for dynamical behaviour. For
  example, you could place the head of an arrow at a fixed plot coordinate, so it always points to
  some important part of your data. The tail of the arrow can be anchored to a text item which
  always resides in the top center of the axis rect, independent of where the user drags the axis
  ranges. This way the arrow stretches and turns so it always points from the label to the
  specified plot coordinate, without any further code necessary.
  
  For a more detailed introduction, see the QCPAbstractItem documentation, and from there the
  documentations of the individual built-in items, to find out how to use them.
  
  \section performancetweaks Performance Tweaks
  
  Although QCustomPlot is quite fast, some features like semi-transparent fills and antialiasing
  can cause a significant slow down. Here are some thoughts on how to increase performance. By far
  the most time is spent in the drawing functions, specifically the drawing of graphs. For maximum
  performance, consider the following (most recommended/effective measures first):
  
  \li use Qt 4.8.0 and up. Performance has doubled or tripled with respect to Qt 4.7.4. However they broke QPainter,
  drawing pixel precise things, e.g. scatters, isn't possible with Qt 4.8.0/1. So it's a performance vs. plot
  quality tradeoff when switching to Qt 4.8.
  \li To increase responsiveness during dragging, consider setting \ref QCustomPlot::setNoAntialiasingOnDrag to true.
  \li On X11 (linux), avoid the (slow) native drawing system, use raster by supplying
  "-graphicssystem raster" as command line argument or calling QApplication::setGraphicsSystem("raster")
  before creating the QApplication object.
  \li On all operating systems, use OpenGL hardware acceleration by supplying "-graphicssystem
  opengl" as command line argument or calling QApplication::setGraphicsSystem("opengl"). If OpenGL
  is available, this will slightly decrease the quality of antialiasing, but extremely increase
  performance especially with alpha (semi-transparent) fills, much antialiasing and a large
  QCustomPlot drawing surface. Note however, that the maximum frame rate might be constrained by
  the vertical sync frequency of your monitor (VSync can be disabled in the graphics card driver
  configuration). So for simple plots (where the potential framerate is far above 60 frames per
  second), OpenGL acceleration might achieve numerically lower frame rates than the other
  graphics systems, because they are not capped at the VSync frequency.
  \li Avoid any kind of alpha (transparency), especially in fills
  \li Avoid any kind of antialiasing, especially in graph lines (see QCustomPlot::setNotAntialiasedElements)
  \li Avoid repeatedly setting the complete data set with QCPGraph::setData. Use QCPGraph::addData instead, if most
  data points stay unchanged, e.g. in a running measurement.
  \li Set the \a copy parameter of the setData functions to false, so only pointers get
  transferred. (Relevant only if preparing data maps with a large number of points, i.e. over 10000)
  
  \section Preprocessor Define Flags
  
  QCustomPlot understands some preprocessor defines that are useful for debugging and compilation:
  <dl>
  <dt>\c QCUSTOMPLOT_COMPILE_LIBRARY
  <dd>Define this flag when you compile QCustomPlot as a shared library (.so/.dll)
  <dt>\c QCUSTOMPLOT_USE_LIBRARY
  <dd>Define this flag before including the header, when using QCustomPlot as a shared library
  <dt>\c QCUSTOMPLOT_CHECK_DATA
  <dd>If this flag is defined, the QCustomPlot plottables will perform data validity checks on every redraw.
      This means they will give qDebug output when you plot \e inf or \e nan values, they will not
      fix your data.
  </dl>

*/

/*! \page classoverview Class Overview
  
  \image latex ClassesOverview.png "Overview of all classes and their relations" width=1.2\textwidth
  \image html ClassesOverview.png "Overview of all classes and their relations"
  
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCustomPlot
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCustomPlot
  \brief The central class of the library, the QWidget which displays the plot and interacts with the user.
  
  For tutorials on how to use QCustomPlot, see the website\n
  http://www.WorksLikeClockWork.com/index.php/components/qt-plotting-widget
*/

/* start of documentation of inline functions */

/*! \fn QRect QCustomPlot::viewport() const
  
  Returns the viewport rect of this QCustomPlot instance. The viewport is the area the plot is
  drawn in, all mechanisms, e.g. margin caluclation take the viewport to be the outer border of the
  plot. The viewport normally is the rect() of the QCustomPlot widget, i.e. a rect with top left
  (0, 0) and size of the QCustomPlot widget.
  
  Don't confuse the viewport with the axisRect. An axisRect is the rect defined by two axes, where
  the graphs/plottables are drawn in. The viewport is larger and contains also the axes themselves, their
  tick numbers, their labels, the plot title etc.
  
  Only when saving to a file (see \ref savePng, savePdf etc.) the viewport is temporarily modified
  to allow saving plots with sizes independent of the current widget size.
*/

/* end of documentation of inline functions */
/* start of documentation of signals */

/*! \fn void QCustomPlot::mouseDoubleClick(QMouseEvent *event)

  This signal is emitted when the QCustomPlot receives a mouse double click event.
*/

/*! \fn void QCustomPlot::mousePress(QMouseEvent *event)

  This signal is emitted when the QCustomPlot receives a mouse press event.
  
  It is emitted before the QCustomPlot handles its range dragging mechanism, so a slot connected to
  this signal can still influence the behaviour e.g. with \ref setRangeDrag or \ref
  setRangeDragAxes.
*/

/*! \fn void QCustomPlot::mouseMove(QMouseEvent *event)

  This signal is emitted when the QCustomPlot receives a mouse move event.
  
  It is emitted before the QCustomPlot handles its range dragging mechanism, so a slot connected to
  this signal can still influence the behaviour e.g. with \ref setRangeDrag.
  
  \warning It is discouraged to change the drag-axes with \ref setRangeDragAxes here, because the
  dragging starting point was saved the moment the mouse was pressed. Thus it only has a sensible
  meaning for the range drag axes that were set at that moment. If you want to change the drag
  axes, consider doing this in the \ref mousePress signal instead.
*/

/*! \fn void QCustomPlot::mouseRelease(QMouseEvent *event)

  This signal is emitted when the QCustomPlot receives a mouse release event.
  
  It is emitted before the QCustomPlot handles its selection mechanism, so a slot connected to this
  signal can still influence the behaviour e.g. with \ref setInteractions or \ref
  QCPAbstractPlottable::setSelectable.
*/

/*! \fn void QCustomPlot::mouseWheel(QMouseEvent *event)

  This signal is emitted when the QCustomPlot receives a mouse wheel event.
  
  It is emitted before the QCustomPlot handles its range zooming mechanism, so a slot connected to
  this signal can still influence the behaviour e.g. with \ref setRangeZoom, \ref setRangeZoomAxes
  or \ref setRangeZoomFactor.
*/

/*! \fn void QCustomPlot::plottableClick(QCPAbstractPlottable *plottable, QMouseEvent *event)
  
  This signal is emitted when a plottable is clicked.

  \a event is the mouse event that caused the click and \a plottable is the plottable that received
  the click.
  
  \see plottableDoubleClick
*/

/*! \fn void QCustomPlot::plottableDoubleClick(QCPAbstractPlottable *plottable, QMouseEvent *event)
  
  This signal is emitted when a plottable is double clicked.
  
  \a event is the mouse event that caused the click and \a plottable is the plottable that received
  the click.
  
  \see plottableClick
*/

/*! \fn void QCustomPlot::itemClick(QCPAbstractItem *item, QMouseEvent *event)
  
  This signal is emitted when an item is clicked.

  \a event is the mouse event that caused the click and \a item is the item that received the
  click.
  
  \see itemDoubleClick
*/

/*! \fn void QCustomPlot::itemDoubleClick(QCPAbstractItem *item, QMouseEvent *event)
  
  This signal is emitted when an item is double clicked.
  
  \a event is the mouse event that caused the click and \a item is the item that received the
  click.
  
  \see itemClick
*/

/*! \fn void QCustomPlot::axisClick(QCPAxis *axis, QCPAxis::SelectablePart part, QMouseEvent *event)
  
  This signal is emitted when an axis is clicked.
  
  \a event is the mouse event that caused the click, \a axis is the axis that received the click and
  \a part indicates the part of the axis that was clicked.
  
  \see axisDoubleClick
*/

/*! \fn void QCustomPlot::axisDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part, QMouseEvent *event)

  This signal is emitted when an axis is double clicked.
  
  \a event is the mouse event that caused the click, \a axis is the axis that received the click and
  \a part indicates the part of the axis that was clicked.
  
  \see axisClick
*/

/*! \fn void QCustomPlot::legendClick(QCPLegend *legend, QCPAbstractLegendItem *item, QMouseEvent *event)

  This signal is emitted when a legend (item) is clicked.
  
  \a event is the mouse event that caused the click, \a legend is the legend that received the
  click and \a item is the legend item that received the click. If only the legend and no item is
  clicked, \a item is 0 (e.g. a click inside the legend padding, which is not part of any item).
  
  \see legendDoubleClick
*/

/*! \fn void QCustomPlot::legendDoubleClick(QCPLegend *legend,  QCPAbstractLegendItem *item, QMouseEvent *event)

  This signal is emitted when a legend (item) is double clicked.
  
  \a event is the mouse event that caused the click, \a legend is the legend that received the
  click and \a item is the legend item that received the click. If only the legend and no item is
  clicked, \a item is 0 (e.g. a click inside the legend padding, which is not part of any item).
  
  \see legendClick
*/

/*! \fn void QCustomPlot:: titleClick(QMouseEvent *event)

  This signal is emitted when the plot title is clicked.
  
  \a event is the mouse event that caused the click.
  
  \see titleDoubleClick
*/

/*! \fn void QCustomPlot::titleDoubleClick(QMouseEvent *event)

  This signal is emitted when the plot title is double clicked.
  
  \a event is the mouse event that caused the click.
  
  \see titleClick
*/

/*! \fn void QCustomPlot::selectionChangedByUser()
  
  This signal is emitted after the user has changed the selection in the QCustomPlot, e.g. by
  clicking. It is not emitted, when the selection state of an object has changed programmatically,
  e.g. by a direct call to setSelected() on a plottable or by calling \ref deselectAll.
  
  See the documentation of \ref setInteractions for how to find out which objects are currently
  selected.
  
  \see setInteractions, QCPAbstractPlottable::selectionChanged, QCPAxis::selectionChanged
*/

/*! \fn void QCustomPlot::beforeReplot()
  
  This signal is emitted immediately before a replot takes place (caused by a call to the slot \ref
  replot).
  
  It is safe to mutually connect the replot slot with this signal on two QCustomPlots to make them
  replot synchronously (i.e. it won't cause an infinite recursion).
  
  \see replot, afterReplot
*/

/*! \fn void QCustomPlot::afterReplot()
  
  This signal is emitted immediately after a replot has taken place (caused by a call to the slot \ref
  replot).
  
  It is safe to mutually connect the replot slot with this signal on two QCustomPlots to make them
  replot synchronously (i.e. it won't cause an infinite recursion).
  
  \see replot, beforeReplot
*/

/* end of documentation of signals */

/*!
  Constructs a QCustomPlot and sets reasonable default values.
  Four axes are created at the bottom, left, top and right sides (xAxis, yAxis, xAxis2, yAxis2),
  however, only the bottom and left axes are set to be visible.
  The legend is also set to be invisible initially.
*/
QCustomPlot::QCustomPlot(QWidget *parent) :
  QWidget(parent),
  mMouseEventElement(0),
  mReplotting(false),
  mPlottingHints(QCP::phCacheLabels)
{
  setAttribute(Qt::WA_NoMousePropagation);
  setAttribute(Qt::WA_OpaquePaintEvent);
  setMouseTracking(true);
  QLocale currentLocale = locale();
  currentLocale.setNumberOptions(QLocale::OmitGroupSeparator);
  setLocale(currentLocale);
  mPaintBuffer = QPixmap(size());
  
  // create initial layers:
  mLayers.append(new QCPLayer(this, "background"));
  mLayers.append(new QCPLayer(this, "grid"));
  mLayers.append(new QCPLayer(this, "main"));
  mLayers.append(new QCPLayer(this, "axes"));
  mLayers.append(new QCPLayer(this, "legend"));
  updateLayerIndices();
  setCurrentLayer("main");
  
  // create initial layout, axis rect and legend:
  mPlotLayout = new QCPLayoutGrid;
  mPlotLayout->setParent(this);
  QCPAxisRect *defaultAxisRect = new QCPAxisRect(this, true);
  xAxis = defaultAxisRect->axis(QCPAxis::atBottom);
  yAxis = defaultAxisRect->axis(QCPAxis::atLeft);
  xAxis2 = defaultAxisRect->axis(QCPAxis::atTop);
  yAxis2 = defaultAxisRect->axis(QCPAxis::atRight);
  mPlotLayout->addElement(0, 0, defaultAxisRect);
  legend = new QCPLegend(this);
  legend->setVisible(false);
  legend->setLayer("axes");
  defaultAxisRect->insetLayout()->addElement(legend, Qt::AlignRight|Qt::AlignTop);
  defaultAxisRect->insetLayout()->setMargins(QMargins(12, 12, 12, 12));
  
  setViewport(rect()); // needs to be called after mPlotLayout has been created
  setNoAntialiasingOnDrag(false);
  setAutoAddPlottableToLegend(true);
  setColor(Qt::white);
  
#ifdef Q_WS_WIN
  setPlottingHint(QCP::phForceRepaint);
#endif
  setAntialiasedElements(QCP::aeNone);
  setNotAntialiasedElements(QCP::aeNone);
  setInteractions(0);
  setMultiSelectModifier(Qt::ControlModifier);
  setSelectionTolerance(8);
  
  replot();
}

QCustomPlot::~QCustomPlot()
{
  clearPlottables();
  clearItems();

  if (mPlotLayout)
  {
    delete mPlotLayout;
    mPlotLayout = 0;
  }
  
  mCurrentLayer = 0;
  qDeleteAll(mLayers); // don't use removeLayer, because it would prevent the last layer to be removed
  mLayers.clear();
}

/*!
  Sets the background color of the QCustomPlot widget.
*/
void QCustomPlot::setColor(const QColor &color)
{
  mColor = color;
}

/*!
  Sets which elements are forcibly drawn antialiased as an or combination of QCP::AntialiasedElement.
  
  This overrides the antialiasing settings for whole element groups, normally controlled with the
  \a setAntialiasing function on the individual elements. If an element is neither specified in
  \ref setAntialiasedElements nor in \ref setNotAntialiasedElements, the antialiasing setting on
  each individual element instance is used.
  
  For example, if \a antialiasedElements contains \ref QCP::aePlottables, all plottables will be
  drawn antialiased, no matter what the specific QCPAbstractPlottable::setAntialiased value was set
  to.
  
  \see setNotAntialiasedElements
*/
void QCustomPlot::setAntialiasedElements(const QCP::AntialiasedElements &antialiasedElements)
{
  mAntialiasedElements = antialiasedElements;
  
  // make sure elements aren't in mNotAntialiasedElements and mAntialiasedElements simultaneously:
  if ((mNotAntialiasedElements & mAntialiasedElements) != 0)
    mNotAntialiasedElements |= ~mAntialiasedElements;
}

/*!
  Sets whether the specified \a antialiasedElement is forcibly drawn antialiased.
  
  This overrides the antialiasing settings for whole element groups, normally controlled with the
  \a setAntialiasing function on the individual elements. If an element is neither specified in
  \ref setAntialiasedElements nor in \ref setNotAntialiasedElements, the antialiasing setting on
  each individual element instance is used.
  
  For example, if \a enabled is true and \a antialiasedElement is \ref QCP::aePlottables, all
  plottables will be drawn antialiased, no matter what the specific
  QCPAbstractPlottable::setAntialiased value was set to.
  
  \see setNotAntialiasedElement
*/
void QCustomPlot::setAntialiasedElement(QCP::AntialiasedElement antialiasedElement, bool enabled)
{
  if (!enabled && mAntialiasedElements.testFlag(antialiasedElement))
    mAntialiasedElements &= ~antialiasedElement;
  else if (enabled && !mAntialiasedElements.testFlag(antialiasedElement))
    mAntialiasedElements |= antialiasedElement;
  
  // make sure elements aren't in mNotAntialiasedElements and mAntialiasedElements simultaneously:
  if ((mNotAntialiasedElements & mAntialiasedElements) != 0)
    mNotAntialiasedElements |= ~mAntialiasedElements;
}

/*!
  Sets which elements are forcibly drawn not antialiased as an or combination of
  QCP::AntialiasedElement.
  
  This overrides the antialiasing settings for whole element groups, normally controlled with the
  \a setAntialiasing function on the individual elements. If an element is neither specified in
  \ref setAntialiasedElements nor in \ref setNotAntialiasedElements, the antialiasing setting on
  each individual element instance is used.
  
  For example, if \a notAntialiasedElements contains \ref QCP::aePlottables, no plottables will be
  drawn antialiased, no matter what the specific QCPAbstractPlottable::setAntialiased value was set
  to.
  
  if an element in \a notAntialiasedElements is already set in \ref setAntialiasedElements, it is
  removed from there.
  
  \see setAntialiasedElements
*/
void QCustomPlot::setNotAntialiasedElements(const QCP::AntialiasedElements &notAntialiasedElements)
{
  mNotAntialiasedElements = notAntialiasedElements;
  
  // make sure elements aren't in mNotAntialiasedElements and mAntialiasedElements simultaneously:
  if ((mNotAntialiasedElements & mAntialiasedElements) != 0)
    mAntialiasedElements |= ~mNotAntialiasedElements;
}

/*!
  Sets whether the specified \a notAntialiasedElement is forcibly drawn not antialiased.
  
  This overrides the antialiasing settings for whole element groups, normally controlled with the
  \a setAntialiasing function on the individual elements. If an element is neither specified in
  \ref setAntialiasedElements nor in \ref setNotAntialiasedElements, the antialiasing setting on
  each individual element instance is used.
  
  For example, if \a enabled is true and \a notAntialiasedElement is \ref QCP::aePlottables, no
  plottables will be drawn antialiased, no matter what the specific
  QCPAbstractPlottable::setAntialiased value was set to.
  
  if \a enabled is true and \a notAntialiasedElement is already set with \ref
  setAntialiasedElement, it is removed from there.
  
  \see setAntialiasedElement
*/
void QCustomPlot::setNotAntialiasedElement(QCP::AntialiasedElement notAntialiasedElement, bool enabled)
{
  if (!enabled && mNotAntialiasedElements.testFlag(notAntialiasedElement))
    mNotAntialiasedElements &= ~notAntialiasedElement;
  else if (enabled && !mNotAntialiasedElements.testFlag(notAntialiasedElement))
    mNotAntialiasedElements |= notAntialiasedElement;
  
  // make sure elements aren't in mNotAntialiasedElements and mAntialiasedElements simultaneously:
  if ((mNotAntialiasedElements & mAntialiasedElements) != 0)
    mAntialiasedElements |= ~mNotAntialiasedElements;
}

/*!
  If set to true, adding a plottable (e.g. a graph) to the QCustomPlot automatically also adds the
  newly created plottable to the legend.
  
  \see addPlottable, addGraph, QCPLegend::addItem
*/
void QCustomPlot::setAutoAddPlottableToLegend(bool on)
{
  mAutoAddPlottableToLegend = on;
}

/*!
  Sets the possible interactions of this QCustomPlot as an or-combination of \ref Interaction
  enums. There are the following types of interactions:
  
  <b>Axis range manipulation</b> is controlled via \ref iRangeDrag and \ref iRangeZoom. When the
  respective interaction is enabled, the user may drag axes ranges and zoom with the mouse wheel.
  For details how to control which axes the user may drag/zoom and in what orientations, see \ref
  setRangeDrag, \ref setRangeZoom, \ref setRangeDragAxes, \ref setRangeZoomAxes.
  
  <b>Plottable selection</b> is controlled by \ref iSelectPlottables. If \ref iSelectPlottables is
  set, the user may select plottables (e.g. graphs, curves, bars,...) by clicking on them or in
  their vicinity, see \ref setSelectionTolerance. Whether the user can actually select a plottable
  can further be restricted with the \ref QCPAbstractPlottable::setSelectable function on the
  specific plottable. To find out whether a specific plottable is selected, call
  QCPAbstractPlottable::selected(). To retrieve a list of all currently selected plottables, call
  \ref selectedPlottables. If you're only interested in QCPGraphs, you may use the convenience
  function \ref selectedGraphs.
  
  <b>Item selection</b> is controlled by \ref iSelectItems. If \ref iSelectItems is set, the user
  may select items (e.g. QCPItemLine, QCPItemText,...) by clicking on them or in their vicinity. To
  find out whether a specific item is selected, call QCPAbstractItem::selected(). To retrieve a
  list of all currently selected items, call \ref selectedItems.
  
  <b>Axis selection</b> is controlled with \ref iSelectAxes. If \ref iSelectAxes is set, the user
  may select parts of the axes by clicking on them. What parts exactly (e.g. Axis base line, tick
  labels, axis label) are selectable can be controlled via \ref QCPAxis::setSelectable for each
  axis. To retrieve a list of all axes that currently contain selected parts, call \ref
  selectedAxes. Which parts of an axis are selected, can be retrieved with QCPAxis::selected().
  
  <b>Legend selection</b> is controlled with \ref iSelectLegend. If this is set, the user may
  select the legend itself or individual items by clicking on them. What parts exactly are
  selectable can be controlled via \ref QCPLegend::setSelectable. To find out whether the legend or
  any child items are selected, check the value of QCPLegend::selected. To find out which child
  items are selected, call \ref QCPLegend::selectedItems.
  
  <b>Plot title selection</b> is controlled with \ref iSelectTitle. If set, the user may select the
  plot title by clicking on it. To find out whether the title is currently selected, call
  QCustomPlot::titleSelected().
  
  If the selection state has changed by user interaction, the \ref selectionChangedByUser signal is
  emitted. Each selectable object additionally emits an individual selectionChanged signal whenever
  their selection state has changed, i.e. not only by user interaction.
  
  To allow multiple objects to be selected by holding the modifier set with \ref
  setMultiSelectModifier, set the flag \ref iMultiSelect.
  
  \note In addition to the selection mechanism presented here, QCustomPlot always emits
  corresponding signals, when an object is clicked or double clicked. see \ref plottableClick and
  \ref plottableDoubleClick for example.
  
  \see setInteraction, setSelectionTolerance
*/
void QCustomPlot::setInteractions(const QCP::Interactions &interactions)
{
  mInteractions = interactions;
}

/*!
  Sets the single \a interaction of this QCustomPlot to \a enabled.
  
  For details about the interaction system, see \ref setInteractions.
  
  \see setInteractions
*/
void QCustomPlot::setInteraction(const QCP::Interaction &interaction, bool enabled)
{
  if (!enabled && mInteractions.testFlag(interaction))
    mInteractions &= ~interaction;
  else if (enabled && !mInteractions.testFlag(interaction))
    mInteractions |= interaction;
}

/*!
  Sets the tolerance that is used when deciding whether a click on the QCustomPlot surface selects
  an object (e.g. a plottable) or not.
  
  If for example the user clicks in the vicinity of the line of a QCPGraph, it's only regarded as a
  potential selection when the minimum distance between the click position and the graph line is
  smaller than \a pixels. Objects that are defined by an area (e.g. QCPBars) only react to clicks
  directly inside the area and ignore this selection tolerance. In other words it only has meaning
  for parts of objects that are too thin to exactly hit with a click and thus need such a
  tolerance.
  
  \see setInteractions, QCPAbstractPlottable::selectTest
*/
void QCustomPlot::setSelectionTolerance(int pixels)
{
  mSelectionTolerance = pixels;
}

/*!
  Sets whether antialiasing is disabled for all elements while the user is dragging axes ranges. If
  many objects, especially plottables, are normally drawn antialiased, this greatly improves
  performance during dragging. Thus it creates a more responsive user experience. As soon as the
  user stops dragging, the last replot is done with normal antialiasing, to restore high image
  quality.
  
  \see setAntialiasedElements, setNotAntialiasedElements
*/
void QCustomPlot::setNoAntialiasingOnDrag(bool enabled)
{
  mNoAntialiasingOnDrag = enabled;
}

/*!
  Sets the plotting hints for this QCustomPlot instance as an or combination of QCP::PlottingHint.
  
  \see setPlottingHint
*/
void QCustomPlot::setPlottingHints(const QCP::PlottingHints &hints)
{
  mPlottingHints = hints;
}

/*!
  Sets the specified plotting \a hint to \a enabled.
  
  \see setPlottingHints
*/
void QCustomPlot::setPlottingHint(QCP::PlottingHint hint, bool enabled)
{
  QCP::PlottingHints newHints = mPlottingHints;
  if (!enabled)
    newHints &= ~hint;
  else
    newHints |= hint;
  
  if (newHints != mPlottingHints)
    setPlottingHints(newHints);
}

/*!
  Sets the keyboard modifier that will be recognized as multi-select-modifier.
  
  If \ref iMultiSelect is specified in \ref setInteractions, the user may select multiple objects
  by clicking on them one after the other while holding down \a modifier.
  
  By default the multi-select-modifier is set to Qt::ControlModifier.
  
  \see setInteractions
*/
void QCustomPlot::setMultiSelectModifier(Qt::KeyboardModifier modifier)
{
  mMultiSelectModifier = modifier;
}

/*!
  Sets the viewport of this QCustomPlot. The Viewport is the area that the top level layout
  (QCustomPlot::plotLayout()) uses as its rect. Normally, the viewport is the entire widget rect.
*/
void QCustomPlot::setViewport(const QRect &rect)
{
  mViewport = rect;
  if (mPlotLayout)
    mPlotLayout->setOuterRect(mViewport);
}

/*!
  Returns the plottable with \a index. If the index is invalid, returns 0.
  
  There is an overloaded version of this function with no parameter which returns the last added
  plottable, see QCustomPlot::plottable()
  
  \see plottableCount, addPlottable
*/
QCPAbstractPlottable *QCustomPlot::plottable(int index)
{
  if (index >= 0 && index < mPlottables.size())
  {
    return mPlottables.at(index);
  } else
  {
    qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
    return 0;
  }
}

/*! \overload
  
  Returns the last plottable, that was added with \ref addPlottable. If there are no plottables in the plot,
  returns 0.
  
  \see plottableCount, addPlottable
*/
QCPAbstractPlottable *QCustomPlot::plottable()
{
  if (!mPlottables.isEmpty())
  {
    return mPlottables.last();
  } else
    return 0;
}

/*!
  Adds the specified plottable to the plot and, if \ref setAutoAddPlottableToLegend is enabled, to the legend.
  QCustomPlot takes ownership of the plottable.
  
  Returns true on success, i.e. when \a plottable wasn't already added to the plot and
  the parent plot of \a plottable is this QCustomPlot (the latter is controlled by what
  axes the plottable was passed in the constructor).
  
  \see plottable, plottableCount, removePlottable, clearPlottables
*/
bool QCustomPlot::addPlottable(QCPAbstractPlottable *plottable)
{
  if (mPlottables.contains(plottable))
  {
    qDebug() << Q_FUNC_INFO << "plottable already added to this QCustomPlot:" << reinterpret_cast<quintptr>(plottable);
    return false;
  }
  if (plottable->parentPlot() != this)
  {
    qDebug() << Q_FUNC_INFO << "plottable not created with this QCustomPlot as parent:" << reinterpret_cast<quintptr>(plottable);
    return false;
  }
  
  mPlottables.append(plottable);
  // possibly add plottable to legend:
  if (mAutoAddPlottableToLegend)
    plottable->addToLegend();
  // special handling for QCPGraphs to maintain the simple graph interface:
  if (QCPGraph *graph = qobject_cast<QCPGraph*>(plottable))
    mGraphs.append(graph);
  if (!plottable->layer()) // usually the layer is already set in the constructor of the plottable (via QCPLayerable constructor)
    plottable->setLayer(currentLayer());
  return true;
}

/*!
  Removes the specified plottable from the plot and, if necessary, from the legend.
  
  Returns true on success.
  
  \see addPlottable, clearPlottables
*/
bool QCustomPlot::removePlottable(QCPAbstractPlottable *plottable)
{
  if (!mPlottables.contains(plottable))
  {
    qDebug() << Q_FUNC_INFO << "plottable not in list:" << reinterpret_cast<quintptr>(plottable);
    return false;
  }
  
  // remove plottable from legend:
  plottable->removeFromLegend();
  // special handling for QCPGraphs to maintain the simple graph interface:
  if (QCPGraph *graph = qobject_cast<QCPGraph*>(plottable))
    mGraphs.removeOne(graph);
  // remove plottable:
  delete plottable;
  mPlottables.removeOne(plottable);
  return true;
}

/*! \overload
  
  Removes the plottable by its \a index.
*/
bool QCustomPlot::removePlottable(int index)
{
  if (index >= 0 && index < mPlottables.size())
    return removePlottable(mPlottables[index]);
  else
  {
    qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
    return false;
  }
}

/*!
  Removes all plottables from the plot (and the legend, if necessary).
  
  Returns the number of plottables removed.
  
  \see removePlottable
*/
int QCustomPlot::clearPlottables()
{
  int c = mPlottables.size();
  for (int i=c-1; i >= 0; --i)
    removePlottable(mPlottables[i]);
  return c;
}

/*!
  Returns the number of currently existing plottables in the plot
  
  \see plottable, addPlottable
*/
int QCustomPlot::plottableCount() const
{
  return mPlottables.size();
}

/*!
  Returns a list of the selected plottables. If no plottables are currently selected, the list is empty.
  
  There is a convenience function if you're only interested in selected graphs, see \ref selectedGraphs.
  
  \see setInteractions, QCPAbstractPlottable::setSelectable, QCPAbstractPlottable::setSelected, selectedGraphs
*/
QList<QCPAbstractPlottable*> QCustomPlot::selectedPlottables() const
{
  QList<QCPAbstractPlottable*> result;
  for (int i=0; i<mPlottables.size(); ++i)
  {
    if (mPlottables.at(i)->selected())
      result.append(mPlottables.at(i));
  }
  return result;
}

/*!
  Returns the plottable at the pixel position \a pos. Plottables that only consist of single lines
  (e.g. graphs) have a tolerance band around them, see \ref setSelectionTolerance.
  If multiple plottables come into consideration, the one closest to \a pos is returned.
  
  If \a onlySelectable is true, only plottables that are selectable
  (QCPAbstractPlottable::setSelectable) are considered.
  
  If there is no plottable at \a pos, the return value is 0.
*/
QCPAbstractPlottable *QCustomPlot::plottableAt(const QPointF &pos, bool onlySelectable) const
{
  QCPAbstractPlottable *resultPlottable = 0;
  double resultDistance = mSelectionTolerance; // only regard clicks with distances smaller than mSelectionTolerance as selections, so initialize with that value
  
  for (int i=0; i<mPlottables.size(); ++i)
  {
    QCPAbstractPlottable *currentPlottable = mPlottables[i];
    if (onlySelectable && !currentPlottable->selectable())
      continue;
    if ((currentPlottable->keyAxis()->axisRect()->rect() & currentPlottable->valueAxis()->axisRect()->rect()).contains(pos.toPoint())) // only consider clicks inside the rect that is spanned by the plottable's key/value axes
    {
      double currentDistance = currentPlottable->selectTest(pos, false);
      if (currentDistance >= 0 && currentDistance < resultDistance)
      {
        resultPlottable = currentPlottable;
        resultDistance = currentDistance;
      }
    }
  }
  
  return resultPlottable;
}

/*!
  Returns whether this QCustomPlot instance contains the \a plottable.
  
  \see addPlottable
*/
bool QCustomPlot::hasPlottable(QCPAbstractPlottable *plottable) const
{
  return mPlottables.contains(plottable);
}

/*!
  Returns the graph with \a index. If the index is invalid, returns 0.
  
  There is an overloaded version of this function with no parameter which returns the last created
  graph, see QCustomPlot::graph()
  
  \see graphCount, addGraph
*/
QCPGraph *QCustomPlot::graph(int index) const
{
  if (index >= 0 && index < mGraphs.size())
  {
    return mGraphs.at(index);
  } else
  {
    qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
    return 0;
  }
}

/*! \overload
  
  Returns the last graph, that was created with \ref addGraph. If there are no graphs in the plot,
  returns 0.
  
  \see graphCount, addGraph
*/
QCPGraph *QCustomPlot::graph() const
{
  if (!mGraphs.isEmpty())
  {
    return mGraphs.last();
  } else
    return 0;
}

/*!
  Creates a new graph inside the plot. If \a keyAxis and \a valueAxis are left unspecified (0), the
  bottom (xAxis) is used as key and the left (yAxis) is used as value. If specified, \a keyAxis and
  \a valueAxis must reside in this QCustomPlot.
  
  \a keyAxis will be used as key axis (typically "x") and \a valueAxis as value axis (typically
  "y") for the graph.
  
  Returns a pointer to the newly created graph.
  
  \see graph, graphCount, removeGraph, clearGraphs
*/
QCPGraph *QCustomPlot::addGraph(QCPAxis *keyAxis, QCPAxis *valueAxis)
{
  if (!keyAxis) keyAxis = xAxis;
  if (!valueAxis) valueAxis = yAxis;
  if (!keyAxis || !valueAxis)
  {
    qDebug() << Q_FUNC_INFO << "can't use default QCustomPlot xAxis or yAxis, because at least one is invalid (has been deleted)";
    return 0;
  }
  if (keyAxis->parentPlot() != this || valueAxis->parentPlot() != this)
  {
    qDebug() << Q_FUNC_INFO << "passed keyAxis or valueAxis doesn't have this QCustomPlot as parent";
    return 0;
  }
  
  QCPGraph *newGraph = new QCPGraph(keyAxis, valueAxis);
  if (addPlottable(newGraph))
  {
    newGraph->setName("Graph "+QString::number(mGraphs.size()));
    return newGraph;
  } else
  {
    delete newGraph;
    return 0;
  }
}

/*!
  Removes the specified \a graph from the plot and, if necessary, from the legend. If
  any other graphs in the plot have a channel fill set towards the removed graph, the channel fill
  property of those graphs is reset to zero (no channel fill).
  
  Returns true on success.
  
  \see clearGraphs
*/
bool QCustomPlot::removeGraph(QCPGraph *graph)
{
  return removePlottable(graph);
}

/*! \overload
  
  Removes the graph by its \a index.
*/
bool QCustomPlot::removeGraph(int index)
{
  if (index >= 0 && index < mGraphs.size())
    return removeGraph(mGraphs[index]);
  else
    return false;
}

/*!
  Removes all graphs from the plot (and the legend, if necessary).
  Returns the number of graphs removed.
  \see removeGraph
*/
int QCustomPlot::clearGraphs()
{
  int c = mGraphs.size();
  for (int i=c-1; i >= 0; --i)
    removeGraph(mGraphs[i]);
  return c;
}

/*!
  Returns the number of currently existing graphs in the plot
  
  \see graph, addGraph
*/
int QCustomPlot::graphCount() const
{
  return mGraphs.size();
}

/*!
  Returns a list of the selected graphs. If no graphs are currently selected, the list is empty.
  
  \note Even if the returned list is empty, it might still be, that there are selected plottables
  in the plot that are not of type QCPGraph (e.g. QCPCurve, QCPBars, etc.), see \ref
  selectedPlottables. Of course, this only applies, if you actually add non-QCPGraph plottables.
  
  \see setInteractions, selectedPlottables, QCPAbstractPlottable::setSelectable, QCPAbstractPlottable::setSelected
*/
QList<QCPGraph*> QCustomPlot::selectedGraphs() const
{
  QList<QCPGraph*> result;
  for (int i=0; i<mGraphs.size(); ++i)
  {
    if (mGraphs.at(i)->selected())
      result.append(mGraphs.at(i));
  }
  return result;
}

/*!
  Returns the item with \a index. If the index is invalid, returns 0.
  
  There is an overloaded version of this function with no parameter which returns the last added
  item, see QCustomPlot::item()
  
  \see itemCount, addItem
*/
QCPAbstractItem *QCustomPlot::item(int index) const
{
  if (index >= 0 && index < mItems.size())
  {
    return mItems.at(index);
  } else
  {
    qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
    return 0;
  }
}

/*! \overload
  
  Returns the last item, that was added with \ref addItem. If there are no items in the plot,
  returns 0.
  
  \see itemCount, addItem
*/
QCPAbstractItem *QCustomPlot::item() const
{
  if (!mItems.isEmpty())
  {
    return mItems.last();
  } else
    return 0;
}

/*!
  Adds the specified item to the plot. QCustomPlot takes ownership of the item.
  
  Returns true on success, i.e. when \a item wasn't already added to the plot and the parent plot
  of \a item is this QCustomPlot.
  
  \see item, itemCount, removeItem, clearItems
*/
bool QCustomPlot::addItem(QCPAbstractItem *item)
{
  if (!mItems.contains(item) && item->parentPlot() == this)
  {
    mItems.append(item);
    return true;
  } else
  {
    qDebug() << Q_FUNC_INFO << "item either already in list or not created with this QCustomPlot as parent:" << reinterpret_cast<quintptr>(item);
    return false;
  }
}

/*!
  Removes the specified item from the plot.
  
  Returns true on success.
  
  \see addItem, clearItems
*/
bool QCustomPlot::removeItem(QCPAbstractItem *item)
{
  if (mItems.contains(item))
  {
    delete item;
    mItems.removeOne(item);
    return true;
  } else
  {
    qDebug() << Q_FUNC_INFO << "item not in list:" << reinterpret_cast<quintptr>(item);
    return false;
  }
}

/*! \overload
  
  Removes the item by its \a index.
*/
bool QCustomPlot::removeItem(int index)
{
  if (index >= 0 && index < mItems.size())
    return removeItem(mItems[index]);
  else
  {
    qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
    return false;
  }
}

/*!
  Removes all items from the plot.
  
  Returns the number of items removed.
  
  \see removeItem
*/
int QCustomPlot::clearItems()
{
  int c = mItems.size();
  for (int i=c-1; i >= 0; --i)
    removeItem(mItems[i]);
  return c;
}

/*!
  Returns the number of currently existing items in the plot
  
  \see item, addItem
*/
int QCustomPlot::itemCount() const
{
  return mItems.size();
}

/*!
  Returns a list of the selected items. If no items are currently selected, the list is empty.
  
  \see setInteractions, QCPAbstractItem::setSelectable, QCPAbstractItem::setSelected
*/
QList<QCPAbstractItem*> QCustomPlot::selectedItems() const
{
  QList<QCPAbstractItem*> result;
  for (int i=0; i<mItems.size(); ++i)
  {
    if (mItems.at(i)->selected())
      result.append(mItems.at(i));
  }
  return result;
}

/*!
  Returns the item at the pixel position \a pos. Items that only consist of single lines (e.g. \ref
  QCPItemLine or \ref QCPItemCurve) have a tolerance band around them, see \ref
  setSelectionTolerance. If multiple items come into consideration, the one closest to \a pos is
  returned.
  
  If \a onlySelectable is true, only items that are selectable (QCPAbstractItem::setSelectable) are
  considered.
  
  If there is no item at \a pos, the return value is 0.
*/
QCPAbstractItem *QCustomPlot::itemAt(const QPointF &pos, bool onlySelectable) const
{
  QCPAbstractItem *resultItem = 0;
  double resultDistance = mSelectionTolerance; // only regard clicks with distances smaller than mSelectionTolerance as selections, so initialize with that value
  
  for (int i=0; i<mItems.size(); ++i)
  {
    QCPAbstractItem *currentItem = mItems[i];
    if (onlySelectable && !currentItem->selectable())
      continue;
    if (!currentItem->clipToAxisRect() || currentItem->clipRect().contains(pos.toPoint())) // only consider clicks inside axis cliprect of the item if actually clipped to it
    {
      double currentDistance = currentItem->selectTest(pos, false);
      if (currentDistance >= 0 && currentDistance < resultDistance)
      {
        resultItem = currentItem;
        resultDistance = currentDistance;
      }
    }
  }
  
  return resultItem;
}

/*!
  Returns whether this QCustomPlot instance contains the \a item.
  
  \see addItem
*/
bool QCustomPlot::hasItem(QCPAbstractItem *item) const
{
  return mItems.contains(item);
}

/*!
  Returns the layer with the specified \a name.
  
  \see addLayer, moveLayer, removeLayer
*/
QCPLayer *QCustomPlot::layer(const QString &name) const
{
  for (int i=0; i<mLayers.size(); ++i)
  {
    if (mLayers.at(i)->name() == name)
      return mLayers.at(i);
  }
  return 0;
}

/*! \overload
  
  Returns the layer by index.
  
  \see addLayer, moveLayer, removeLayer
*/
QCPLayer *QCustomPlot::layer(int index) const
{
  if (index >= 0 && index < mLayers.size())
  {
    return mLayers.at(index);
  } else
  {
    qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
    return 0;
  }
}

/*!
  Returns the layer that is set as current layer (see \ref setCurrentLayer).
*/
QCPLayer *QCustomPlot::currentLayer() const
{
  return mCurrentLayer; 
}

/*!
  Sets the layer with the specified \a name to be the current layer. All newly created/added
  layerables (\ref QCPLayerable), e.g. plottables and items, are initially placed on the current
  layer.
  
  Returns true on success, i.e. if there is a layer with the specified \a name in the QCustomPlot.
  
  \see addLayer, moveLayer, removeLayer
*/
bool QCustomPlot::setCurrentLayer(const QString &name)
{
  if (QCPLayer *newCurrentLayer = layer(name))
  {
    return setCurrentLayer(newCurrentLayer);
  } else
  {
    qDebug() << Q_FUNC_INFO << "layer with name doesn't exist:" << name;
    return false;
  }
}

/*! \overload
  
  Sets the provided \a layer to be the current layer.
  
  Returns true on success, i.e. when \a layer is a valid layer in the QCustomPlot.
  
  \see addLayer, moveLayer, removeLayer
*/
bool QCustomPlot::setCurrentLayer(QCPLayer *layer)
{
  if (!mLayers.contains(layer))
  {
    qDebug() << Q_FUNC_INFO << "layer not a layer of this QCustomPlot:" << reinterpret_cast<quintptr>(layer);
    return false;
  }
  
  mCurrentLayer = layer;
  return true;
}

/*!
  Returns the number of currently existing layers in the plot
  
  \see layer, addLayer
*/
int QCustomPlot::layerCount() const
{
  return mLayers.size();
}

/*!
  Adds a new layer to this QCustomPlot instance. The new layer will have the name \a name, which must
  be unique. It is positioned either below or above \a otherLayer, which can be controlled with \a insertMode.
  
  Returns true on success, i.e. if there is no other layer named \a name and \a otherLayer is a
  valid layer inside this QCustomPlot.
  
  If \a otherLayer is 0, the highest layer in the QCustomPlot will be used.
  
  For an explanation of what layers are in QCustomPlot, see the documentation of \ref QCPLayer.
  
  \see layer, moveLayer, removeLayer
*/
bool QCustomPlot::addLayer(const QString &name, QCPLayer *otherLayer, QCustomPlot::LayerInsertMode insertMode)
{
  if (!otherLayer)
    otherLayer = mLayers.last();
  if (!mLayers.contains(otherLayer))
  {
    qDebug() << Q_FUNC_INFO << "otherLayer not a layer of this QCustomPlot:" << reinterpret_cast<quintptr>(otherLayer);
    return false;
  }
  if (layer(name))
  {
    qDebug() << Q_FUNC_INFO << "A layer exists already with the name" << name;
    return false;
  }
    
  QCPLayer *newLayer = new QCPLayer(this, name);
  mLayers.insert(otherLayer->index() + (insertMode==limAbove ? 1:0), newLayer);
  updateLayerIndices();
  return true;
}

/*!
  Removes the specified \a layer and returns true on success.
  
  All layerables (e.g. plottables and items) on the removed layer will be moved to the layer below
  \a layer. If \a layer is the bottom layer, the layerables are moved to the layer above. In both
  cases, the total rendering order of all layerables in the QCustomPlot is preserved.
  
  If \a layer is the current layer (\ref setCurrentLayer), the layer below (or above, if bottom
  layer) becomes the new current layer.
  
  Note that it is not possible to remove the last layer.
  
  \see layer, addLayer, moveLayer
*/
bool QCustomPlot::removeLayer(QCPLayer *layer)
{
  if (!mLayers.contains(layer))
  {
    qDebug() << Q_FUNC_INFO << "layer not a layer of this QCustomPlot:" << reinterpret_cast<quintptr>(layer);
    return false;
  }
  if (mLayers.size() < 2)
  {
    qDebug() << Q_FUNC_INFO << "can't remove last layer";
    return false;
  }
  
  // append all children of this layer to layer below (if this is lowest layer, prepend to layer above)
  int removedIndex = layer->index();
  bool isFirstLayer = removedIndex==0;
  QCPLayer *targetLayer = isFirstLayer ? mLayers.at(removedIndex+1) : mLayers.at(removedIndex-1);
  QList<QCPLayerable*> children = layer->children();
  if (isFirstLayer) // prepend in reverse order (so order relative to each other stays the same)
  {
    for (int i=children.size()-1; i>=0; --i)
      children.at(i)->moveToLayer(targetLayer, true);
  } else  // append normally
  {
    for (int i=0; i<children.size(); ++i)
      children.at(i)->moveToLayer(targetLayer, false);
  }
  // if removed layer is current layer, change current layer to layer below/above:
  if (layer == mCurrentLayer)
    setCurrentLayer(targetLayer);
  // remove layer:
  delete layer;
  mLayers.removeOne(layer);
  updateLayerIndices();
  return true;
}

/*!
  Moves the specified \a layer to the position relative to \a otherLayer. Whether \a layer is
  placed above or below \a otherLayer can be controlled with \a insertMode.
  
  Returns true on success, i.e. when both \a layer and \a otherLayer are valid layers in the
  QCustomPlot.
  
  \see layer, addLayer, moveLayer
*/
bool QCustomPlot::moveLayer(QCPLayer *layer, QCPLayer *otherLayer, QCustomPlot::LayerInsertMode insertMode)
{
  if (!mLayers.contains(layer))
  {
    qDebug() << Q_FUNC_INFO << "layer not a layer of this QCustomPlot:" << reinterpret_cast<quintptr>(layer);
    return false;
  }
  if (!mLayers.contains(otherLayer))
  {
    qDebug() << Q_FUNC_INFO << "otherLayer not a layer of this QCustomPlot:" << reinterpret_cast<quintptr>(otherLayer);
    return false;
  }
  
  mLayers.move(layer->index(), otherLayer->index() + (insertMode==limAbove ? 1:0));
  updateLayerIndices();
  return true;
}

int QCustomPlot::axisRectCount() const
{
  return axisRects().size();
}

QCPAxisRect *QCustomPlot::axisRect(int index) const
{
  const QList<QCPAxisRect*> rectList = axisRects();
  if (index >= 0 && index < rectList.size())
  {
    return rectList.at(index);
  } else
  {
    qDebug() << Q_FUNC_INFO << "invalid axis rect index" << index;
    return 0;
  }
}

QList<QCPAxisRect*> QCustomPlot::axisRects() const
{
  QList<QCPAxisRect*> result;
  QStack<QCPLayoutElement*> elementStack;
  if (mPlotLayout)
    elementStack.push(mPlotLayout);
  
  while (!elementStack.isEmpty())
  {
    QList<QCPLayoutElement*> subElements = elementStack.pop()->elements();
    for (int i=0; i<subElements.size(); ++i)
    {
      if (QCPLayoutElement *element = subElements.at(i))
      {
        elementStack.push(element);
        if (QCPAxisRect *ar = qobject_cast<QCPAxisRect*>(element))
          result.append(ar);
      }
    }
  }
  
  return result;
}

QCPLayoutElement *QCustomPlot::layoutElementAt(const QPointF &pos) const
{
  QCPLayoutElement *current = mPlotLayout;
  bool searchSubElements = true;
  while (searchSubElements && current)
  {
    searchSubElements = false;
    const QList<QCPLayoutElement*> elements = current->elements();
    for (int i=0; i<elements.size(); ++i)
    {
      if (elements.at(i) && elements.at(i)->hitTest(pos))
      {
        current = elements.at(i);
        searchSubElements = true;
        break;
      }
    }
  }
  return current;
}

/*!
  Returns the axes that currently have selected parts, i.e. whose selection is not \ref QCPAxis::spNone.
  
  \see selectedPlottables, selectedLegends, setInteractions, QCPAxis::setSelected, QCPAxis::setSelectable
*/
QList<QCPAxis*> QCustomPlot::selectedAxes() const
{
  QList<QCPAxis*> result, allAxes;
  QList<QCPAxisRect*> rects = axisRects();
  for (int i=0; i<rects.size(); ++i)
    allAxes << rects.at(i)->axes();
  
  for (int i=0; i<allAxes.size(); ++i)
  {
    if (allAxes.at(i)->selectedParts() != QCPAxis::spNone)
      result.append(allAxes.at(i));
  }
  
  return result;
}

/*!
  Returns the legends (typically one or zero) that currently have selected parts, i.e. whose
  selection is not \ref QCPLegend::spNone.
  
  \see selectedPlottables, selectedAxes, setInteractions, QCPLegend::setSelected, QCPLegend::setSelectable, QCPLegend::selectedItems
*/
QList<QCPLegend*> QCustomPlot::selectedLegends() const
{
  QList<QCPLegend*> result;
  
  QStack<QCPLayoutElement*> elementStack;
  if (mPlotLayout)
    elementStack.push(mPlotLayout);
  
  while (!elementStack.isEmpty())
  {
    QList<QCPLayoutElement*> subElements = elementStack.pop()->elements();
    for (int i=0; i<subElements.size(); ++i)
    {
      if (QCPLayoutElement *element = subElements.at(i))
      {
        elementStack.push(element);
        if (QCPLegend *leg = qobject_cast<QCPLegend*>(element))
        {
          if (leg->selectedParts() != QCPLegend::spNone)
            result.append(leg);
        }
      }
    }
  }
  
  return result;
}

/*!
  Deselects plottables, items, axes and legends of the QCustomPlot. You may pass a list of
  QCPLayerables as \a exceptions whose selection state will not be modified.
  
  Since calling this function is not a user interaction, this does not emit the \ref
  selectionChangedByUser signal. The individual selectionChanged signals are emitted though, if the
  objects were previously selected.
  
  \see setInteractions, selectedPlottables, selectedItems, selectedAxes, selectedLegends
*/
void QCustomPlot::deselectAll()
{
  for (int i=0; i<mLayers.size(); ++i)
  {
    QList<QCPLayerable*> layerables = mLayers.at(i)->children();
    for (int k=0; k<layerables.size(); ++k)
      layerables.at(k)->deselectEvent(0);
  }
}

/*!
  Causes a complete replot (axes, labels, graphs, etc.) into the internal buffer. Finally, update()
  is called, to redraw the buffer on the QCustomPlot widget surface.
  
  Before the replot happens, the signal \ref beforeReplot is emitted. After the replot, \ref afterReplot is
  emitted. It is safe to mutually connect the replot slot with any of those two signals on two QCustomPlots
  to make them replot synchronously (i.e. it won't cause an infinite recursion).
*/
void QCustomPlot::replot()
{
  if (mReplotting) // incase signals loop back to replot slot
    return;
  mReplotting = true;
  emit beforeReplot();
  mPaintBuffer.fill(mColor);
  QCPPainter painter;
  painter.begin(&mPaintBuffer);
  if (painter.isActive()) 
  {
    painter.setRenderHint(QPainter::HighQualityAntialiasing); // to make Antialiasing look good if using the OpenGL graphicssystem
    draw(&painter);
    painter.end();
    if (mPlottingHints.testFlag(QCP::phForceRepaint))
      repaint();
    else
      update();
  } else // might happen if QCustomPlot has width or height zero
    qDebug() << Q_FUNC_INFO << "Couldn't activate painter on buffer";
  emit afterReplot();
  mReplotting = false;
}

/*!
  Rescales the axes such that all plottables (e.g. graphs) in the plot are fully visible.
  It does this by calling \ref QCPAbstractPlottable::rescaleAxes on all plottables.
  
  \see QCPAbstractPlottable::rescaleAxes
*/
void QCustomPlot::rescaleAxes()
{
  if (mPlottables.isEmpty()) return;
  
  mPlottables.at(0)->rescaleAxes(false); // onlyEnlarge disabled on first plottable
  for (int i=1; i<mPlottables.size(); ++i)
    mPlottables.at(i)->rescaleAxes(true);  // onlyEnlarge enabled on all other plottables
}

/*!
  Saves a PDF with the vectorized plot to the file \a fileName. The axis ratio as well as the scale
  of texts and lines will be derived from the specified \a width and \a height. This means, the
  output will look like the normal on-screen output of a QCustomPlot widget with the corresponding
  pixel width and height. If either \a width or \a height is zero, the exported image will have
  the same dimensions as the QCustomPlot widget currently has.

  \a noCosmeticPen disables the use of cosmetic pens when drawing to the PDF file. Cosmetic pens
  are pens with numerical width 0, which are always drawn as a one pixel wide line, no matter what
  zoom factor is set in the PDF-Viewer. For more information about cosmetic pens, see QPainter and
  QPen documentation.
  
  The objects of the plot will appear in the current selection state. So when you don't want e.g.
  selected axes to be painted in their selected look, deselect everything with \ref deselectAll
  before calling this function.

  Returns true on success.
  
  \warning
  \li If you plan on editing the exported PDF file with a vector graphics editor like
  Inkscape, it is advised to set \a noCosmeticPen to true to avoid losing those cosmetic lines
  (which might be quite many, because cosmetic pens are the default for e.g. axes and tick marks).
  \li If calling this function inside the constructor of the parent of the QCustomPlot widget
  (i.e. the MainWindow constructor, if QCustomPlot is inside the MainWindow), always provide
  explicit non-zero widths and heights. If you leave \a width or \a height as 0 (default), this
  function uses the current width and height of the QCustomPlot widget. However, in Qt, these
  aren't defined yet inside the constructor, so you would get an image that has strange
  widths/heights.
  
  \see savePng, saveBmp, saveJpg, saveRastered
*/
bool QCustomPlot::savePdf(const QString &fileName, bool noCosmeticPen, int width, int height)
{
  bool success = false;
  int newWidth, newHeight;
  if (width == 0 || height == 0)
  {
    newWidth = this->width();
    newHeight = this->height();
  } else
  {
    newWidth = width;
    newHeight = height;
  }
  
  QPrinter printer(QPrinter::ScreenResolution);
  printer.setOutputFileName(fileName);
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setFullPage(true);
  QRect oldViewport = viewport();
  setViewport(QRect(0, 0, newWidth, newHeight));
  printer.setPaperSize(viewport().size(), QPrinter::DevicePixel);
  QCPPainter printpainter;
  if (printpainter.begin(&printer))
  {
    printpainter.setMode(QCPPainter::pmVectorized);
    printpainter.setMode(QCPPainter::pmNoCaching);
    printpainter.setWindow(mViewport);
    printpainter.setRenderHint(QPainter::NonCosmeticDefaultPen, noCosmeticPen);
    if (mColor != Qt::white && mColor != Qt::transparent && mColor.alpha() > 0) // draw pdf background color if not white/transparent
      printpainter.fillRect(viewport(), mColor);
    draw(&printpainter);
    printpainter.end();
    success = true;
  }
  setViewport(oldViewport);
  return success;
}

/*!
  Saves a PNG image file to \a fileName on disc. The output plot will have the dimensions \a width
  and \a height in pixels. If either \a width or \a height is zero, the exported image will have
  the same dimensions as the QCustomPlot widget currently has. Line widths and texts etc. are not
  scaled up when larger widths/heights are used. If you want that effect, use the \a scale parameter.

  For example, if you set both \a width and \a height to 100 and \a scale to 2, you will end up with an
  image file of size 200*200 in which all graphical elements are scaled up by factor 2 (line widths,
  texts, etc.). This scaling is not done by stretching a 100*100 image, the result will have full
  200*200 pixel resolution.

  \warning If calling this function inside the constructor of the parent of the QCustomPlot widget
  (i.e. the MainWindow constructor, if QCustomPlot is inside the MainWindow), always provide
  explicit non-zero widths and heights. If you leave \a width or \a height as 0 (default), this
  function uses the current width and height of the QCustomPlot widget. However, in Qt, these
  aren't defined yet inside the constructor, so you would get an image that has strange
  widths/heights.
  
  The objects of the plot will appear in the current selection state. If you don't want any selected
  objects to be painted in their selected look, deselect everything with \ref deselectAll before calling
  this function.

  If you want the plot to be painted in a PNG with transparent background, call \ref setColor with a
  transparent color, e.g. Qt::transparent, before saving.

  PNG compression can be controlled with the \a quality parameter which must be between 0 and 100 or
  -1 to use the default setting.
  
  Returns true on success. If this function fails, most likely the PNG format isn't supported by
  the system, see Qt docs about QImageWriter::supportedImageFormats().

  \see savePdf, saveBmp, saveJpg, saveRastered
*/
bool QCustomPlot::savePng(const QString &fileName, int width, int height, double scale, int quality)
{  
  return saveRastered(fileName, width, height, scale, "PNG", quality);
}

/*!
  Saves a JPG image file to \a fileName on disc. The output plot will have the dimensions \a width
  and \a height in pixels. If either \a width or \a height is zero, the exported image will have
  the same dimensions as the QCustomPlot widget currently has. Line widths and texts etc. are not
  scaled up when larger widths/heights are used. If you want that effect, use the \a scale parameter.

  For example, if you set both \a width and \a height to 100 and \a scale to 2, you will end up with an
  image file of size 200*200 in which all graphical elements are scaled up by factor 2 (line widths,
  texts, etc.). This scaling is not done by stretching a 100*100 image, the result will have full
  200*200 pixel resolution.

  \warning If calling this function inside the constructor of the parent of the QCustomPlot widget
  (i.e. the MainWindow constructor, if QCustomPlot is inside the MainWindow), always provide
  explicit non-zero widths and heights. If you leave \a width or \a height as 0 (default), this
  function uses the current width and height of the QCustomPlot widget. However, in Qt, these
  aren't defined yet inside the constructor, so you would get an image that has strange
  widths/heights.

  The objects of the plot will appear in the current selection state. If you don't want any selected
  objects to be painted in their selected look, deselect everything with \ref deselectAll before calling
  this function.

  JPG compression can be controlled with the \a quality parameter which must be between 0 and 100 or
  -1 to use the default setting.
  
  Returns true on success. If this function fails, most likely the JPG format isn't supported by
  the system, see Qt docs about QImageWriter::supportedImageFormats().

  \see savePdf, savePng, saveBmp, saveRastered
*/
bool QCustomPlot::saveJpg(const QString &fileName, int width, int height, double scale, int quality)
{
  return saveRastered(fileName, width, height, scale, "JPG", quality);
}

/*!
  Saves a BMP image file to \a fileName on disc. The output plot will have the dimensions \a width
  and \a height in pixels. If either \a width or \a height is zero, the exported image will have
  the same dimensions as the QCustomPlot widget currently has. Line widths and texts etc. are not
  scaled up when larger widths/heights are used. If you want that effect, use the \a scale parameter.

  For example, if you set both \a width and \a height to 100 and \a scale to 2, you will end up with an
  image file of size 200*200 in which all graphical elements are scaled up by factor 2 (line widths,
  texts, etc.). This scaling is not done by stretching a 100*100 image, the result will have full
  200*200 pixel resolution.

  \warning If calling this function inside the constructor of the parent of the QCustomPlot widget
  (i.e. the MainWindow constructor, if QCustomPlot is inside the MainWindow), always provide
  explicit non-zero widths and heights. If you leave \a width or \a height as 0 (default), this
  function uses the current width and height of the QCustomPlot widget. However, in Qt, these
  aren't defined yet inside the constructor, so you would get an image that has strange
  widths/heights.

  The objects of the plot will appear in the current selection state. If you don't want any selected
  objects to be painted in their selected look, deselect everything with \ref deselectAll before calling
  this function.
  
  Returns true on success. If this function fails, most likely the BMP format isn't supported by
  the system, see Qt docs about QImageWriter::supportedImageFormats().

  \see savePdf, savePng, saveJpg, saveRastered
*/
bool QCustomPlot::saveBmp(const QString &fileName, int width, int height, double scale)
{
  return saveRastered(fileName, width, height, scale, "BMP");
}

/*! \internal
  
  Returns a minimum size hint that corresponds to the minimum size of the top level layout
  (plotLayout()). To prevent QCustomPlot from being collapsed to size/width zero, set a minimum
  size (setMinimumSize) either on the whole QCustomPlot or on any layout elements inside the plot.
  This is especially important, when placed in a QLayout where other components try to take in as
  much space as possible (e.g. QMdiArea).
*/
QSize QCustomPlot::minimumSizeHint() const
{
  return mPlotLayout->minimumSizeHint();
}

/*! \internal
  
  Returns a size hint that is the same as minimumSizeHint().
  
*/
QSize QCustomPlot::sizeHint() const
{
  return mPlotLayout->minimumSizeHint();
}

/*! \internal
  
  Event handler for when the QCustomPlot widget needs repainting. This does not cause a replot, but
  draws the internal buffer on the widget surface.
*/
void QCustomPlot::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event);
  QPainter painter(this);
  painter.drawPixmap(0, 0, mPaintBuffer);
}

/*! \internal
  
  Event handler for a resize of the QCustomPlot widget. Causes the internal buffer to be resized to
  the new size. The viewport (which becomes the outer rect of mPlotLayout) is resized
  appropriately. Finally a replot is performed.
*/
void QCustomPlot::resizeEvent(QResizeEvent *event)
{
  // resize and repaint the buffer:
  mPaintBuffer = QPixmap(event->size());
  setViewport(rect());
  replot();
}

/*! \internal
  
 Event handler for when a double click occurs. Emits the mouseDoubleClick signal, then emits the
 specialized signals when certain objecs are clicked (e.g. plottableDoubleClick, axisDoubleClick,
 etc.). Finally determines the affected layout element and forwards the event to it.
 
 \see mousePressEvent, mouseReleaseEvent
*/
void QCustomPlot::mouseDoubleClickEvent(QMouseEvent *event)
{
  emit mouseDoubleClick(event);
  
  QVariant details;
  QCPLayerable *clickedLayerable = layerableAt(event->pos(), false, &details);
  
  // emit specialized object double click signals:
  if (QCPAbstractPlottable *ap = dynamic_cast<QCPAbstractPlottable*>(clickedLayerable))
    emit plottableDoubleClick(ap, event);
  else if (QCPAxis *ax = dynamic_cast<QCPAxis*>(clickedLayerable))
    emit axisDoubleClick(ax, details.value<QCPAxis::SelectablePart>(), event);
  else if (QCPAbstractItem *ai = dynamic_cast<QCPAbstractItem*>(clickedLayerable))
    emit itemDoubleClick(ai, event);
  else if (QCPLegend *lg = dynamic_cast<QCPLegend*>(clickedLayerable))
    emit legendDoubleClick(lg, 0, event);
  else if (QCPAbstractLegendItem *li = dynamic_cast<QCPAbstractLegendItem*>(clickedLayerable))
    emit legendDoubleClick(li->parentLegend(), li, event);
  else if (QCPPlotTitle *pt = dynamic_cast<QCPPlotTitle*>(clickedLayerable))
    emit titleDoubleClick(event, pt);
  
  // call event of affected layout element:
  if (QCPLayoutElement *el = layoutElementAt(event->pos()))
    el->mouseDoubleClickEvent(event);
  
  QWidget::mouseDoubleClickEvent(event);
}

/*! \internal
  
  Event handler for when a mouse button is pressed. Emits the mousePress signal. Then determines
  the affected layout element and forwards the event to it.
  
  \see mouseMoveEvent, mouseReleaseEvent
*/
void QCustomPlot::mousePressEvent(QMouseEvent *event)
{
  emit mousePress(event);
  mMousePressPos = event->pos(); // need this to determine in releaseEvent whether it was a click (no position change between press and release)
  
  // call event of affected layout element:
  mMouseEventElement = layoutElementAt(event->pos());
  if (mMouseEventElement)
    mMouseEventElement->mousePressEvent(event);
  
  QWidget::mousePressEvent(event);
}

/*! \internal
  
  Event handler for when the cursor is moved. Emits the mouseMove signal.

  If a layout element has mouse capture focus (a mousePressEvent happened on top of the layout
  element before), the mouseMoveEvent is forwarded to that element.
  
  \see mousePressEvent, mouseReleaseEvent
*/
void QCustomPlot::mouseMoveEvent(QMouseEvent *event)
{
  emit mouseMove(event);

  // call event of affected layout element:
  if (mMouseEventElement)
    mMouseEventElement->mouseMoveEvent(event);
  
  QWidget::mouseMoveEvent(event);
}

/*! \internal
  
  Event handler for when a mouse button is released. Emits the mouseRelease signal.
  
  If the mouse was moved less than a certain threshold in any direction since the mousePressEvent,
  it is considered a click which causes the selection mechanism (if activated via \ref
  setInteractions) to possibly change selection states accordingly. Further, specialized mouse
  click signals are emitted (e.g. plottableClick, axesClick, etc.)
  
  If a layout element has mouse capture focus (a mousePressEvent happened on top of the layout
  element before), the mouseReleaseEvent is forwarded to that element.
  
  \see mousePressEvent, mouseMoveEvent
*/
void QCustomPlot::mouseReleaseEvent(QMouseEvent *event)
{
  emit mouseRelease(event);
  bool doReplot = false;
  
  if ((mMousePressPos-event->pos()).manhattanLength() < 5) // determine whether it was a click operation
  {
    if (event->button() == Qt::LeftButton)
    {
      // handle selection mechanism:
      QVariant details;
      QCPLayerable *clickedLayerable = layerableAt(event->pos(), true, &details);
      bool selectionStateChanged = false;
      bool additive = mInteractions.testFlag(QCP::iMultiSelect) && event->modifiers().testFlag(mMultiSelectModifier);
      if (clickedLayerable && mInteractions.testFlag(clickedLayerable->selectionCategory()))
      {
        // a layerable was actually clicked, call its selectEvent:
        bool selChanged = false;
        clickedLayerable->selectEvent(event, additive, details, &selChanged);
        selectionStateChanged |= selChanged;
      }
      // deselect all other layerables if not additive selection:
      if (!additive)
      {
        for (int i=0; i<mLayers.size(); ++i)
        {
          QList<QCPLayerable*> layerables = mLayers.at(i)->children();
          for (int k=0; k<layerables.size(); ++k)
          {
            if (layerables.at(k) != clickedLayerable && mInteractions.testFlag(layerables.at(k)->selectionCategory()))
            {
              bool selChanged = false;
              layerables.at(k)->deselectEvent(&selChanged);
              selectionStateChanged |= selChanged;
            }
          }
        }
      }
      doReplot = true;
      if (selectionStateChanged)
        emit selectionChangedByUser();
    }
    
    // emit specialized object click signals:
    QVariant details;
    QCPLayerable *clickedLayerable = layerableAt(event->pos(), false, &details); // for these signals, selectability is ignored, that's why we call this again with onlySelectable set to false
    if (QCPAbstractPlottable *ap = dynamic_cast<QCPAbstractPlottable*>(clickedLayerable))
      emit plottableClick(ap, event);
    else if (QCPAxis *ax = dynamic_cast<QCPAxis*>(clickedLayerable))
      emit axisClick(ax, details.value<QCPAxis::SelectablePart>(), event);
    else if (QCPAbstractItem *ai = dynamic_cast<QCPAbstractItem*>(clickedLayerable))
      emit itemClick(ai, event);
    else if (QCPLegend *lg = dynamic_cast<QCPLegend*>(clickedLayerable))
      emit legendClick(lg, 0, event);
    else if (QCPAbstractLegendItem *li = dynamic_cast<QCPAbstractLegendItem*>(clickedLayerable))
      emit legendClick(li->parentLegend(), li, event);
    else if (QCPPlotTitle *pt = dynamic_cast<QCPPlotTitle*>(clickedLayerable))
      emit titleClick(event, pt);
  }
  
  // call event of affected layout element:
  if (mMouseEventElement)
  {
    mMouseEventElement->mouseReleaseEvent(event);
    mMouseEventElement = 0;
  }
  
  if (doReplot || noAntialiasingOnDrag())
    replot();
  
  QWidget::mouseReleaseEvent(event);
}

/*! \internal
  
  Event handler for mouse wheel events. First, the mouseWheel signal is emitted. Then determines
  the affected layout element and forwards the event to it.
  
*/
void QCustomPlot::wheelEvent(QWheelEvent *event)
{
  emit mouseWheel(event);
  
  // call event of affected layout element:
  if (QCPLayoutElement *el = layoutElementAt(event->pos()))
    el->wheelEvent(event);
  
  QWidget::wheelEvent(event);
}

/*! \internal
  
  This is the main draw function which first generates the tick vectors of all axes,
  calculates and applies appropriate margins if autoMargin is true and finally draws
  all elements with the passed \a painter. (axis background, title, subgrid, grid, axes, plottables)
*/
void QCustomPlot::draw(QCPPainter *painter)
{
  // recalculate layout (this also updates tick vectors on axes via QCPAxisRect::update):
  mPlotLayout->update();
  
  /*
  // draw axis backgrounds:
  for (int i=0; i<axisRectList.size(); ++i)
    axisRectList.at(i)->drawBackground(painter);
  */
    
  // draw all layered objects (grid, axes, plottables, items, legend,...):
  for (int layerIndex=0; layerIndex < mLayers.size(); ++layerIndex)
  {
    QList<QCPLayerable*> layerChildren = mLayers.at(layerIndex)->children();
    for (int k=0; k < layerChildren.size(); ++k)
    {
      QCPLayerable *child = layerChildren.at(k);
      if (child->visible())
      {
        painter->save();
        painter->setClipRect(child->clipRect().translated(0, -1));
        child->applyDefaultAntialiasingHint(painter);
        child->draw(painter);
        painter->restore();
      }
    }
  }
}

void QCustomPlot::axisRemoved(QCPAxis *axis)
{
  if (xAxis == axis)
    xAxis = 0;
  if (xAxis2 == axis)
    xAxis2 = 0;
  if (yAxis == axis)
    yAxis = 0;
  if (yAxis2 == axis)
    yAxis2 = 0;
  
  // Note: No need to take care of range drag axes and range zoom axes, because they are stored in smart pointers
}

void QCustomPlot::legendRemoved(QCPLegend *legend)
{
  if (this->legend == legend)
    this->legend = 0;
}

void QCustomPlot::updateLayerIndices() const
{
  for (int i=0; i<mLayers.size(); ++i)
    mLayers.at(i)->mIndex = i;
}

QCPLayerable *QCustomPlot::layerableAt(const QPointF &pos, bool onlySelectable, QVariant *selectionDetails) const
{
  for (int layerIndex=mLayers.size()-1; layerIndex>=0; --layerIndex)
  {
    const QList<QCPLayerable*> layerables = mLayers.at(layerIndex)->children();
    double minimumDistance = selectionTolerance()*1.1;
    QCPLayerable *minimumDistanceLayerable = 0;
    for (int i=layerables.size()-1; i>=0; --i)
    {
      if (!layerables.at(i)->visible())
        continue;
      QVariant details;
      double dist = layerables.at(i)->selectTest(pos, onlySelectable, &details);
      if (dist >= 0 && dist < minimumDistance)
      {
        minimumDistance = dist;
        minimumDistanceLayerable = layerables.at(i);
        if (selectionDetails) *selectionDetails = details;
      }
    }
    if (minimumDistance < selectionTolerance())
      return minimumDistanceLayerable;
  }
  return 0;
}

/*!
  Saves the plot to a rastered image file \a fileName in the image format \a
  format. The plot is sized to \a width and \a height in pixels and scaled with
  \a scale. (width 100 and scale 2.0 lead to a full resolution file with width
  200.) If the \a format supports compression, \a quality may be between 0 and
  100 to control it.
  
  Returns true on success. If this function fails, most likely the given \a format isn't supported
  by the system, see Qt docs about QImageWriter::supportedImageFormats().
  
  \see saveBmp, saveJpg, savePng, savePdf
*/
bool QCustomPlot::saveRastered(const QString &fileName, int width, int height, double scale, const char *format, int quality)
{
  QPixmap buffer = pixmap(width, height, scale);
  if (!buffer.isNull())
    return buffer.save(fileName, format, quality);
  else
    return false;
}

/*!
  Renders the plot to a pixmap and returns it.
  
  The plot is sized to \a width and \a height in pixels and scaled with \a scale. (width 100 and
  scale 2.0 lead to a full resolution pixmap with width 200.)
  
  \see saveRastered, saveBmp, savePng, saveJpg, savePdf
*/
QPixmap QCustomPlot::pixmap(int width, int height, double scale)
{
  int newWidth, newHeight;
  if (width == 0 || height == 0)
  {
    newWidth = this->width();
    newHeight = this->height();
  } else
  {
    newWidth = width;
    newHeight = height;
  }
  int scaledWidth = qRound(scale*newWidth);
  int scaledHeight = qRound(scale*newHeight);

  QPixmap result(scaledWidth, scaledHeight);
  result.fill(mColor);
  QCPPainter painter;
  painter.begin(&result);
  if (painter.isActive())
  {
    QRect oldViewport = viewport();
    setViewport(QRect(0, 0, newWidth, newHeight));
    painter.setMode(QCPPainter::pmNoCaching);
    if (!qFuzzyCompare(scale, 1.0))
    {
      if (scale > 1.0) // for scale < 1 we always want cosmetic pens where possible, because else lines would disappear
      {
        painter.setMode(QCPPainter::pmScaledPen);
        painter.setRenderHint(QPainter::NonCosmeticDefaultPen);
      }
      painter.scale(scale, scale);
    }
    draw(&painter);
    setViewport(oldViewport);
    painter.end();
  } else // might happen if pixmap has width or height zero
  {
    qDebug() << Q_FUNC_INFO << "Couldn't activate painter on pixmap";
    return QPixmap();
  }
  return result;
}
