# make file for transparent26 test scripts

T26_SCRIPTS= 1-get-current-mode 2-enable-transparent-mode 3-smartcard-scan 4-send-PKOC-select 5-send-PKOC-authenticate
T26_SCRIPT_ETC= authenticate-PKOC.json enable-transparent-mode.json get-current-mode.json select-PKOC.json smartcard-scan.json
T26_ROOT=opt/transparent26

all:	${T26_SCRIPTS}
	mkdir -p ../${T26_ROOT}/bin
	mkdir -p ../${T26_ROOT}/etc/settings
	cp ${T26_SCRIPTS} ../${T26_ROOT}/bin
	(cd etc; cp ${T26_SCRIPT_ETC} ../../${T26_ROOT}/etc/settings)

clean:
	echo remove opt from the top

