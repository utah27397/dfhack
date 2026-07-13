#include "Core.h"
#include "Console.h"
#include "Export.h"
#include "PluginManager.h"

#include "DataDefs.h"
#include "df/world.h"
#include "df/ui.h"
#include "df/building_nest_boxst.h"
#include "df/building_type.h"
#include "df/buildings_other_id.h"
#include "df/global_objects.h"
#include "df/item.h"
#include "df/item_eggst.h"
#include "df/unit.h"
#include "df/building.h"
#include "df/items_other_id.h"
#include "df/creature_raw.h"
#include "modules/MapCache.h"
#include "modules/Items.h"
#include "modules/Job.h"


using std::vector;
using std::string;
using std::endl;
using namespace DFHack;
using namespace df::enums;

using df::global::world;
using df::global::ui;

static command_result nestboxes(color_ostream &out, vector <string> & parameters);

DFHACK_PLUGIN("nestboxes");

DFHACK_PLUGIN_IS_ENABLED(enabled);

static void eggscan(color_ostream &out)
{
    CoreSuspender suspend;

    for (df::building *build : world->buildings.other[df::buildings_other_id::NEST_BOX])
    {
        auto type = build->getType();
        if (df::enums::building_type::NestBox == type)
        {
            df::building_nest_boxst *nb = virtual_cast<df::building_nest_boxst>(build);
            // The first contained item is the nestbox construction material.
            for (size_t j = 1; j < nb->contained_items.size(); j++)
            {
                auto *item = virtual_cast<df::item_eggst>(nb->contained_items[j]->item);
                if (!item)
                    continue;

                bool fertile = item->egg_flags.bits.fertile;
                if (item->flags.bits.forbid != fertile)
                {
                    item->flags.bits.forbid = fertile;
                    if (fertile && item->flags.bits.in_job) {
                        auto job_ref = Items::getSpecificRef(item, specific_ref_type::JOB);
                        if (job_ref && job_ref->data.job)
                            Job::removeJob(job_ref->data.job);
                    }
                    out << item->getStackSize() << " eggs " << (fertile ? "forbidden" : "unforbidden.") << endl;
                }
            }
        }
    }
}


DFhackCExport command_result plugin_init (color_ostream &out, std::vector <PluginCommand> &commands)
{
    if (world && ui) {
        commands.push_back(
            PluginCommand(
                "nestboxes",
                "Protect fertile eggs incubating in a nestbox.",
                nestboxes));
    }
    return CR_OK;
}

DFhackCExport command_result plugin_shutdown ( color_ostream &out )
{
    return CR_OK;
}

DFhackCExport command_result plugin_onupdate(color_ostream &out)
{
    if (!enabled)
        return CR_OK;

    static unsigned cnt = 0;
    if ((++cnt % 5) != 0)
        return CR_OK;

    eggscan(out);

    return CR_OK;
}

DFhackCExport command_result plugin_enable(color_ostream &out, bool enable)
{
    enabled = enable;
    return CR_OK;
}

static command_result nestboxes(color_ostream &out, vector <string> & parameters)
{
    CoreSuspender suspend;

    if (parameters.size() == 1) {
        if (parameters[0] == "enable")
            enabled = true;
        else if (parameters[0] == "disable")
            enabled = false;
        else
            return CR_WRONG_USAGE;
    } else {
        out << "Plugin " << (enabled ? "enabled" : "disabled") << "." << endl;
    }
    return CR_OK;
}
