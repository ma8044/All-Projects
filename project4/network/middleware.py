# network/middleware.py

import logging
from django.http import HttpResponse

logger = logging.getLogger(__name__)

class BrokenPipeMiddleware:
    def __init__(self, get_response):
        self.get_response = get_response

    def __call__(self, request):
        response = None
        try:
            response = self.get_response(request)
        except IOError as e:
            if 'broken pipe' in str(e).lower():
                logger.warning('Broken pipe error: {}'.format(e))
            else:
                logger.error('IOError: {}'.format(e))
            response = HttpResponse(status=500)
        return response