#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Import logging for recording logs."""
import logging

def init_logging():
    """
    Initialize logging
    """
    # logging.basicConfig(level=logging.INFO, format='%(levelname)-8s %(message)s')
    logging.basicConfig(
        level=logging.INFO, format="%(asctime)s - %(levelname)8s - %(message)s"
    )
