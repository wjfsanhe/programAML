#include "atsc_types.h"
#include "atsc_descriptor.h"
#include "atsc_tvct.h"

#define MAKE_SHORT_HL(exp)		            ((INT16U)((exp##_hi<<8)|exp##_lo))
#define MAKE_WORD_HML(exp)			((INT32U)(((exp##_hi)<<24)|((exp##_mh)<<16)|((exp##_ml)<<8)|exp##_lo))

#define MAJOR_CHANNEL_NUM(exp) ((INT16U)((exp##_hi<<6)|exp##_lo))
#define MINOR_CHANNEL_NUM(exp) ((INT16U)((exp##_hi<<8)|exp##_lo))

static tvct_section_info_t *new_tvct_section_info(void)
{
    tvct_section_info_t *tmp = NULL;

    tmp = AMMem_malloc(sizeof(tvct_section_info_t));
    if(tmp)
    {
        tmp->transport_stream_id = DVB_INVALID_ID;
        tmp->version_number = DVB_INVALID_VERSION;
        tmp->num_channels_in_section = 0;
        tmp->vct_chan_info = NULL;
        tmp->p_next = NULL;
    }

    return tmp;
}

static void add_tvct_section_info(tvct_section_info_t **head, tvct_section_info_t *info)
{
    tvct_section_info_t **tmp = head;
    tvct_section_info_t **temp = head;

    if(info && head)
    {
        if(*tmp == NULL)
        {
            info->p_next = NULL;
            *tmp = info;
        }
        else
        {
            while(*tmp)
            {
                if(*tmp == info)
                {
                    return ;
                }

                temp = tmp;
                tmp = &(*tmp)->p_next;
            }

            info->p_next = NULL;
            (*temp)->p_next = info;

            return ;
        }
    }

    return ;
}

void add_tvct_channel_info(tvct_channel_info_t **head, tvct_channel_info_t *info)
{
	tvct_channel_info_t **tmp = head;
	tvct_channel_info_t **node = head;

	if (info && head)
	{
		if (*tmp == NULL)
		{
			info->p_next = NULL;
			*tmp = info;
		}
		else
		{
			while (*tmp)
			{
				if (*tmp == info)
				{
					return ;
				}
				node = tmp;
				tmp = &(*tmp)->p_next;
			}
			info->p_next =NULL;
			(*node)->p_next = info;
			return ;
		}
	}
	return ;
}

INT32S atsc_psip_parse_tvct(INT8U* data, INT32U length, tvct_section_info_t *info)
{
	INT8U *sect_data = data;
	INT32U sect_len = length;
	tvct_section_info_t *sect_info = info;
	tvct_section_header_t *sect_head = NULL;
	tvct_sect_chan_info_t *vct_sect_chan = NULL;
	tvct_channel_info_t *tmp_chan_info = NULL;
	INT8U num_channels_in_section = 0;
	INT8U *ptr = NULL;

	if(data && length && info)
	{
		while(sect_len > 0)
		{
			sect_head = (tvct_section_header_t*)sect_data;

			if(sect_head->table_id != ATSC_PSIP_TVCT_TID)
			{
				sect_len -= 3;
				sect_len -= MAKE_SHORT_HL(sect_head->section_length);

				sect_data += 3;
				sect_data += MAKE_SHORT_HL(sect_head->section_length);

				continue;
			}

			if(NULL == sect_info)
			{
				sect_info = new_tvct_section_info();
				if(sect_info == NULL)
				{
					AM_TRACE("[%s] error out of memory !!\n", __FUNC__);
					break;
				}
				else
				{
					add_tvct_section_info(&info, sect_info);
				}
			}

			sect_info->transport_stream_id = MAKE_SHORT_HL(sect_head->transport_stream_id);
			sect_info->version_number = sect_head->version_number;

			num_channels_in_section = sect_head->num_channels_in_section;
			sect_info->num_channels_in_section= num_channels_in_section;
			ptr = sect_data+TVCT_SECTION_HEADER_LEN;
			while (num_channels_in_section)
			{
				vct_sect_chan = (tvct_sect_chan_info_t *)ptr;
				tmp_chan_info= (tvct_channel_info_t *)AMMem_malloc(sizeof(tvct_channel_info_t));
				if (tmp_chan_info)
				{
					memset(tmp_chan_info, 0, sizeof(tvct_channel_info_t));
					short_channel_name_parse(ptr, tmp_chan_info->short_name);
					tmp_chan_info->major_channel_number = MAJOR_CHANNEL_NUM(vct_sect_chan->major_channel_number);
					tmp_chan_info->minor_channel_number = MINOR_CHANNEL_NUM(vct_sect_chan->minor_channel_number);
					tmp_chan_info->program_number = MAKE_SHORT_HL(vct_sect_chan->program_number);
					tmp_chan_info->service_type = vct_sect_chan->service_type;
					tmp_chan_info->source_id = MAKE_SHORT_HL(vct_sect_chan->source_id);
					tmp_chan_info->carrier_frequency = MAKE_WORD_HML(vct_sect_chan->carrier_frequency);
					tmp_chan_info->modulation_mode = vct_sect_chan->modulation_mode;
					tmp_chan_info->access_controlled = vct_sect_chan->access_controlled;
					tmp_chan_info->hidden = vct_sect_chan->hidden;
					tmp_chan_info->hide_guide = vct_sect_chan->hide_guide;
					tmp_chan_info->desc = NULL;
					tmp_chan_info->p_next = NULL;

					add_tvct_channel_info(&sect_info->vct_chan_info, tmp_chan_info);
				}
				else
				{
					AM_TRACE("[%s] error out of memory !!\n", __FUNC__);
					break;
				}         
				ptr += 32 + MAKE_SHORT_HL(vct_sect_chan->descriptors_length);
				num_channels_in_section--;
			}

			sect_len -= 3;
			sect_len -= MAKE_SHORT_HL(sect_head->section_length);

			sect_data += 3;
			sect_data += MAKE_SHORT_HL(sect_head->section_length);

			sect_info = sect_info->p_next;
		}

		return 0;
	}

	return -1;
}

void atsc_psip_clear_tvct_info(tvct_section_info_t *info)
{
	tvct_section_info_t *vct_sect_info = NULL;
	tvct_section_info_t *tmp_vct_sect_info = NULL;

	if(info)
	{
		tmp_vct_sect_info = info->p_next;

		while(tmp_vct_sect_info)
		{
			tvct_channel_info_t *chan_info = NULL;
			tvct_channel_info_t *tmp_chan_info = NULL;
			tmp_chan_info = tmp_vct_sect_info->vct_chan_info;
			while(tmp_chan_info)
			{
				if(tmp_chan_info->desc)
				{
					AMMem_free(tmp_chan_info->desc);
					tmp_chan_info->desc = NULL;
				}
				chan_info = tmp_chan_info->p_next;
				AMMem_free(tmp_chan_info);
				tmp_chan_info = chan_info;
			}
			vct_sect_info = tmp_vct_sect_info->p_next;
			AMMem_free(tmp_vct_sect_info);
			tmp_vct_sect_info = vct_sect_info;
		}
		if(info->vct_chan_info)
		{
			AMMem_free(info->vct_chan_info);
		}
		
		info->transport_stream_id = DVB_INVALID_ID;
		info->version_number = DVB_INVALID_VERSION;
		info->num_channels_in_section = 0;
		info->vct_chan_info = NULL;
		info->p_next = NULL;
	}
	return ;
}

tvct_section_info_t *atsc_psip_new_tvct_info(void)
{
    return new_tvct_section_info();
}

void atsc_psip_free_tvct_info(tvct_section_info_t *info)
{
	tvct_section_info_t *vct_sect_info = NULL;
	tvct_section_info_t *tmp_vct_sect_info = NULL;
	if (info)
	{
		tmp_vct_sect_info = info;
		while(tmp_vct_sect_info)
		{
			tvct_channel_info_t *vct_chan_info = NULL;
			tvct_channel_info_t *tmp_vct_chan_info =NULL;
			tmp_vct_chan_info = tmp_vct_sect_info->vct_chan_info;
			while(tmp_vct_chan_info)
			{
				if (tmp_vct_chan_info->desc)
				{
					AMMem_free(tmp_vct_chan_info->desc);
					tmp_vct_chan_info->desc = NULL;
				}
				vct_chan_info = tmp_vct_chan_info->p_next;
				AMMem_free(tmp_vct_chan_info);
				tmp_vct_chan_info = vct_chan_info;
			}
			vct_sect_info = tmp_vct_sect_info->p_next;
			AMMem_free(tmp_vct_sect_info);
			tmp_vct_sect_info = vct_sect_info;
		}
	}
       return ;
}

void atsc_psip_dump_tvct_info(tvct_section_info_t *info)
{
#ifndef __ROM_
	tvct_section_info_t *tmp = info;

	INT8U i = 0;

	if(info)
	{
		AM_TRACE("\r\n============= TVCT INFO =============\r\n");

		while(tmp)
		{ 
			tvct_channel_info_t *chan_info = tmp->vct_chan_info;
			AM_TRACE("transport_stream_id: 0x%04x\r\n", tmp->transport_stream_id);
			AM_TRACE("version_number: 0x%02x\r\n", tmp->version_number);
			AM_TRACE("num_channels_in_section: %d\r\n", tmp->num_channels_in_section); 

			while(chan_info)
			{
				
				AM_TRACE("    %d-%d %s program number:%d source id %d\n", 
					chan_info->major_channel_number,
					chan_info->minor_channel_number,
					chan_info->short_name,
					chan_info->program_number,
					chan_info->source_id);
				
				chan_info = chan_info->p_next;
			}

			tmp = tmp->p_next;
		}

		AM_TRACE("\r\n=============== END ================\r\n");
	}
#endif
    return ;
}

