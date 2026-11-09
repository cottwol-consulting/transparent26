/*
  apdu's for various transparent mode tests

  (C)Copyright 2026 Cottwol Consulting

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#define XWR_APDU_PKOC_SELECT \
"{\"command\":\"xwrite\",\"mode\":\"01\",\"pcmnd\":\"01\",\"pdata\":\"0000a4040008A00000089800000100\"}"

#define XWR_APDU_PKOC_AUTHENTICATE \
"{\"command\":\"xwrite\",\"mode\":\"01\",\"pcmnd\":\"01\",\"pdata\":\"0080800001385C0201004C10171717171717171717171717171717174D20535353535353535353535353535353537272727272727272727272727272727200\"}"

