TEMPLATE = app

CONFIG += qt warn_on debug

QT += opengl network widgets

# qmake selects gcc for cp files
# Some of the code depends on C++11 standard so set it here
QMAKE_CC = g++ -std=gnu++0x

# libraries needed to compile
LIBS += -rdynamic -pthread -ldl -lz -lgsl -lgslcblas -lGL -lGLU -lpython2.7

INCLUDEPATH += . \
               agent \
               app \
               brain \
	       brain/groups \
	       brain/sheets \
               complexity \
               critter \
               environment \
               genome \
	       genome/groups \
	       genome/sheets \
               graphics \
	       logs \
	       monitor \
               network \
	       proplib \
               ui \
	       ui/gui \
               ui/network \
	       ui/term \
               utils \
	       /usr/include/python2.7/

HEADERS += app/Simulation.h \ 
    app/simtypes.h \
    app/simconst.h \
    app/globals.h \
    app/debug.h \
    app/Scheduler.h \
    app/GeneStats.h \
    app/FittestList.h \
    app/EatStatistics.h \
    app/Domain.h \
    agent/agent.h \
    agent/SpeedSensor.h \
    agent/Retina.h \
    agent/RandomSensor.h \
    agent/Metabolism.h \
    agent/MateWaitSensor.h \
    agent/LifeSpan.h \
    agent/EnergySensor.h \
    agent/CarryingSensor.h \
    agent/BeingCarriedSensor.h \
    agent/AgentPovRenderer.h \
    agent/AgentListener.h \
    agent/AgentAttachedData.h \
    brain/SpikingModel.h \
    brain/Sensor.h \
    brain/NeuronModel.h \
    brain/NeuralNetRenderer.h \
    brain/NervousSystem.h \
    brain/Nerve.h \
    brain/FiringRateModel.h \
    brain/Brain.h \
    brain/BaseNeuronModel.h \
    brain/groups/GroupsNeuralNetRenderer.h \
    brain/groups/GroupsBrain.h \
    brain/sheets/SheetsModel.h \
    brain/sheets/SheetsBrain.h \
    complexity/complexity_motion.h \
    complexity/complexity_brain.h \
    complexity/complexity_algorithm.h \
    complexity/complexity.h \
    complexity/adami.h \
    debugger/DebuggerWindow.h \
    debugger/BehaviorPanel.h \
    environment/food.h \
    environment/brick.h \
    environment/barrier.h \
    environment/Patch.h \
    environment/FoodType.h \
    environment/FoodPatch.h \
    environment/Energy.h \
    environment/BrickPatch.h \
    genome/SeparationCache.h \
    genome/NeuronType.h \
    genome/NeurGroupType.h \
    genome/GenomeUtil.h \
    genome/GenomeSchema.h \
    genome/GenomeLayout.h \
    genome/Genome.h \
    genome/GeneSchema.h \
    genome/Gene.h \
    genome/groups/GroupsSynapseType.h \
    genome/groups/GroupsGenomeSchema.h \
    genome/groups/GroupsGenome.h \
    genome/groups/GroupsGene.h \
    genome/sheets/SheetsGenomeSchema.h \
    genome/sheets/SheetsGenome.h \
    genome/sheets/SheetsCrossover.h \
    graphics/gstage.h \
    graphics/gsquare.h \
    graphics/gscene.h \
    graphics/grect.h \
    graphics/graphics.h \
    graphics/gpolygon.h \
    graphics/gpoint.h \
    graphics/gobject.h \
    graphics/gmisc.h \
    graphics/gline.h \
    graphics/glight.h \
    graphics/gcamera.h \
    logs/Logs.h \
    logs/Logger.h \
    monitor/SceneRenderer.h \
    monitor/MovieController.h \
    monitor/MonitorManager.h \
    monitor/Monitor.h \
    monitor/CameraController.h \
    monitor/AgentTracker.h \
    proplib/writer.h \
    proplib/state.h \
    proplib/schema.h \
    proplib/proplib.h \
    proplib/parser.h \
    proplib/overlay.h \
    proplib/interpreter.h \
    proplib/expression.h \
    proplib/editor.h \
    proplib/dom.h \
    proplib/cppprops.h \
    proplib/convert.h \
    proplib/builder.h \
    ui/SimulationController.h \
    ui/gui/ToggleWidgetOpenAction.h \
    ui/gui/StatusTextMonitorView.h \
    ui/gui/SceneMonitorView.h \
    ui/gui/PovMonitorView.h \
    ui/gui/MonitorView.h \
    ui/gui/MainWindow.h \
    ui/gui/ChartMonitorView.h \
    ui/gui/BrainMonitorView.h \
    ui/gui/BinChartViewMonitor.h \
    ui/term/termio.h \
    ui/term/TerminalUI.h \
    ui/term/Prompt.h \
    utils/objectxsortedlist.h \
    utils/objectlist.h \
    utils/next_combination.h \
    utils/misc.h \
    utils/indexlist.h \
    utils/graybin.h \
    utils/gdlink.h \
    utils/error.h \
    utils/distributions.h \
    utils/datalib.h \
    utils/Variant.h \
    utils/Scalar.h \
    utils/Resources.h \
    utils/RandomNumberGenerator.h \
    utils/Queue.h \
    utils/PwMovieUtils.h \
    utils/Mutex.h \
    utils/Events.h \
    utils/AbstractFile.h \
    ui/network/NetworkDialog.h \
    network/NetworkServer.h \
    network/NetworkThread.h \
    network/SendWorldFileThread.h
           
SOURCES += app/Simulation.cp \
    app/simtypes.cp \
    app/globals.cp \
    app/debug.cp \
    app/Scheduler.cp \
    app/GeneStats.cp \
    app/FittestList.cp \
    app/EatStatistics.cp \
    agent/agent.cp \
    agent/SpeedSensor.cp \
    agent/Retina.cp \
    agent/RandomSensor.cp \
    agent/Metabolism.cp \
    agent/MateWaitSensor.cp \
    agent/LifeSpan.cp \
    agent/EnergySensor.cp \
    agent/CarryingSensor.cp \
    agent/BeingCarriedSensor.cp \
    agent/AgentPovRenderer.cp \
    agent/AgentAttachedData.cp \
    brain/SpikingModel.cp \
    brain/NervousSystem.cp \
    brain/Nerve.cp \
    brain/FiringRateModel.cp \
    brain/Brain.cp \
    brain/groups/GroupsBrain.cp \
    brain/sheets/SheetsModel.cp \
    brain/sheets/SheetsBrain.cp \
    complexity/complexity_motion.cp \
    complexity/complexity_brain.cp \
    complexity/complexity_algorithm.cp \
    complexity/adami.cp \
    debugger/DebuggerWindow.cp \
    debugger/BehaviorPanel.cp \
    environment/food.cp \
    environment/brick.cp \
    environment/barrier.cp \
    environment/Patch.cp \
    environment/FoodType.cp \
    environment/FoodPatch.cp \
    environment/Energy.cp \
    environment/BrickPatch.cp \
    genome/SeparationCache.cp \
    genome/GenomeUtil.cp \
    genome/GenomeSchema.cp \
    genome/GenomeLayout.cp \
    genome/Genome.cp \
    genome/GeneSchema.cp \
    genome/Gene.cp \
    genome/groups/GroupsSynapseType.cp \
    genome/groups/GroupsGenomeSchema.cp \
    genome/groups/GroupsGenome.cp \
    genome/groups/GroupsGene.cp \
    genome/sheets/SheetsGenomeSchema.cp \
    genome/sheets/SheetsGenome.cp \
    genome/sheets/SheetsCrossover.cp \
    graphics/gstage.cp \
    graphics/gsquare.cp \
    graphics/gscene.cp \
    graphics/grect.cp \
    graphics/graphics.cp \
    graphics/gpolygon.cp \
    graphics/gpoint.cp \
    graphics/gobject.cp \
    graphics/gmisc.cp \
    graphics/gline.cp \
    graphics/glight.cp \
    graphics/gcamera.cp \
    logs/Logs.cp \
    logs/Logger.cp \
    main/main.cp \
    monitor/SceneRenderer.cp \
    monitor/MovieController.cp \
    monitor/MonitorManager.cp \
    monitor/Monitor.cp \
    monitor/CameraController.cp \
    monitor/AgentTracker.cp \
    proplib/writer.cp \
    proplib/state.cp \
    proplib/schema.cp \
    proplib/parser.cp \
    proplib/overlay.cp \
    proplib/interpreter.cp \
    proplib/expression.cp \
    proplib/editor.cp \
    proplib/dom.cp \
    proplib/cppprops.cp \
    proplib/convert.cp \
    proplib/builder.cp \
    ui/SimulationController.cp \
    ui/gui/ToggleWidgetOpenAction.cp \
    ui/gui/StatusTextMonitorView.cp \
    ui/gui/SceneMonitorView.cp \
    ui/gui/PovMonitorView.cp \
    ui/gui/MonitorView.cp \
    ui/gui/MainWindow.cp \
    ui/gui/ChartMonitorView.cp \
    ui/gui/BrainMonitorView.cp \
    ui/term/termio.cp \
    ui/term/TerminalUI.cp \
    ui/term/Prompt.cp \
    utils/objectxsortedlist.cp \
    utils/misc.cp \
    utils/indexlist.cp \
    utils/error.cp \
    utils/distributions.cp \
    utils/datalib.cp \
    utils/Variant.cp \
    utils/Scalar.cp \
    utils/Resources.cp \
    utils/RandomNumberGenerator.cp \
    utils/PwMovieUtils.cp \
    utils/Mutex.cp \
    utils/AbstractFile.cp \
    ui/network/NetworkDialog.cpp \
    network/NetworkServer.cp \
    network/NetworkThread.cpp \
    network/SendWorldFileThread.cpp

OTHER_FILES += \
    Makefile \
    scripts/build/SConstruct \
    scripts/build/scons_util.py \
    scripts/build/SConstruct2 \
    sandbox/social/feed_young.wf \
    etc/worldfile.wfs


