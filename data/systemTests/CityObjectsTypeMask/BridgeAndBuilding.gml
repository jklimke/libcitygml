<?xml version="1.0" encoding="utf-8"?>
<core:CityModel xmlns:gml="http://www.opengis.net/gml" xmlns:core="http://www.opengis.net/citygml/2.0" xmlns:brid="http://www.opengis.net/citygml/bridge/2.0" xmlns:bldg="http://www.opengis.net/citygml/building/2.0">
	<gml:boundedBy>
		<gml:Envelope srsName="EPSG:23632" srsDimension="3">
			<gml:lowerCorner>457644.585 5439126.455 109.0</gml:lowerCorner>
			<gml:upperCorner>457708.084 5439157.169 124.3</gml:upperCorner>
		</gml:Envelope>
	</gml:boundedBy>
	<core:cityObjectMember>
		<brid:Bridge>
			<brid:lod2MultiSurface>
				<gml:MultiSurface>
					<gml:surfaceMember>
						<gml:Polygon>
							<gml:exterior>
								<gml:LinearRing>
									<gml:posList srsDimension="3">
									0 0 0
									0 10 0
									10 10 0
									10 0 0
									0 0 0
									</gml:posList>
								</gml:LinearRing>
							</gml:exterior>
						</gml:Polygon>
					</gml:surfaceMember>
				</gml:MultiSurface>
			</brid:lod2MultiSurface>
		</brid:Bridge>
	</core:cityObjectMember>
	<core:cityObjectMember>
		<bldg:Building>
			<bldg:consistsOfBuildingPart>
				<bldg:BuildingPart>
					<bldg:boundedBy>
						<bldg:WallSurface>
							<bldg:lod2MultiSurface>
								<gml:MultiSurface>
									<gml:surfaceMember>
										<gml:CompositeSurface>
											<gml:surfaceMember>
												<gml:Polygon>
													<gml:exterior>
														<gml:LinearRing>
															<gml:posList srsDimension="3">
															15 0 0
															15 10 0
															25 10 0
															25 0 0
															15 0 0
															</gml:posList>
														</gml:LinearRing>
													</gml:exterior>
												</gml:Polygon>
											</gml:surfaceMember>
										</gml:CompositeSurface>
									</gml:surfaceMember>
								</gml:MultiSurface>
							</bldg:lod2MultiSurface>
						</bldg:WallSurface>
					</bldg:boundedBy>
				</bldg:BuildingPart>
			</bldg:consistsOfBuildingPart>
		</bldg:Building>
	</core:cityObjectMember>
</core:CityModel>